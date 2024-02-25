# Long term RPC Transaction History

There's a need for RPC to serve at least 6 months of transaction history. The
current history, on the order of days, is insufficient for downstream users.

6 months of transaction data cannot be stored practically in a validator's
rocksdb ledger so an external data store is necessary. The validator's rocksdb
ledger will continue to serve as the primary data source, and then will fall
back to the external data store.

The affected RPC endpoints are:

- [getFirstAvailableBlock](https://solana.com/docs/rpc/http/getfirstavailableblock)
- [getConfirmedBlock](https://solana.com/docs/rpc/deprecated/getconfirmedblock)
- [getConfirmedBlocks](https://solana.com/docs/rpc/deprecated/getconfirmedblocks)
- [getConfirmedSignaturesForAddress](https://solana.com/docs/rpc/http/getconfirmedsignaturesforaddress)
- [getConfirmedTransaction](https://solana.com/docs/rpc/deprecated/getConfirmedTransaction)
- [getSignatureStatuses](https://solana.com/docs/rpc/http/getsignaturestatuses)
- [getBlockTime](https://solana.com/docs/rpc/http/getblocktime)

Some system design constraints:

- The volume of data to store and search can quickly jump into the terabytes,
  and is immutable.
- The system should be as light as possible for SREs. For example an SQL
  database cluster that requires an SRE to continually monitor and rebalance
  nodes is undesirable.
- Data must be searchable in real time - batched queries that take minutes or
  hours to run are unacceptable.
- Easy to replicate the data worldwide to co-locate it with the RPC endpoints
  that will utilize it.
- Interfacing with the external data store should be easy and not require
  depending on risky lightly-used community-supported code libraries

Based on these constraints, Google's BigTable product is selected as the data
store.

## Table Schema

A BigTable instance is used to hold all transaction data, broken up into
different tables for quick searching.

New data may be copied into the instance at anytime without affecting the
existing data, and all data is immutable. Generally the expectation is that new
data will be uploaded once an current epoch completes but there is no limitation
on the frequency of data dumps.

Cleanup of old data is automatic by configuring the data retention policy of the
instance tables appropriately, it just disappears. Therefore the order of when
data is added becomes important. For example if data from epoch N-1 is added
after data from epoch N, the older epoch data will outlive the newer data.
However beyond producing _holes_ in query results, this kind of unordered
deletion will have no ill effect. Note that this method of cleanup effectively
allows for an unlimited amount of transaction data to be stored, restricted only
by the monetary costs of doing so.

The table layout s supports the existing RPC endpoints only. New RPC endpoints
in the future may require additions to the schema and potentially iterating over
all transactions to build up the necessary metadata.

## Accessing BigTable

BigTable has a gRPC endpoint that can be accessed using the
[tonic](https://crates.io/crates/crate)] and the raw protobuf API, as currently
no higher-level Rust crate for BigTable exists. Practically this makes parsing
the results of BigTable queries more complicated but is not a significant issue.

## Data Population

The ongoing population of instance data will occur on an epoch cadence through
the use of a new `solana-ledger-tool` command that will convert rocksdb data for
a given slot range into the instance schema.

The same process will be run once, manually, to backfill the existing ledger
data.

### Block Table: `block`

This table contains the compressed block data for a given slot.

The row key is generated by taking the 16 digit lower case hexadecimal
representation of the slot, to ensure that the oldest slot with a confirmed
block will always be first when the rows are listed. eg, The row key for slot 42
would be 000000000000002a.

The row data is a compressed `StoredConfirmedBlock` struct.

### Account Address Transaction Signature Lookup Table: `tx-by-addr`

This table contains the transactions that affect a given address.

The row key is
`<base58 address>/<slot-id-one's-compliment-hex-slot-0-prefixed-to-16-digits>`.
The row data is a compressed `TransactionByAddrInfo` struct.

Taking the one's compliment of the slot allows for listing of slots ensures that
the newest slot with transactions that affect an address will always be listed
first.

Sysvar addresses are not indexed. However frequently used programs such as Vote
or System are, and will likely have a row for every confirmed slot.

### Transaction Signature Lookup Table: `tx`

This table maps a transaction signature to its confirmed block, and index within
that block.

The row key is the base58-encoded transaction signature.
The row data is a compressed `TransactionInfo` struct.

### Entries Table: `entries`

> Support for the `entries` table was added in v1.18.0.

This table contains data about the entries in a slot.

The row key is the same as a `block` row key.

The row data is a compressed `Entries` struct, which is a list of entry-summary
data, including hash, number of hashes since previous entry, number of
transactions, and starting transaction index.