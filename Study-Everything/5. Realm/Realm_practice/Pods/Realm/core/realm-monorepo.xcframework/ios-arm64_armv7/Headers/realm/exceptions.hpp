/*************************************************************************
 *
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#ifndef REALM_EXCEPTIONS_HPP
#define REALM_EXCEPTIONS_HPP

#include <stdexcept>

#include <realm/util/features.h>
#include <realm/util/backtrace.hpp>
#include <realm/util/to_string.hpp>

namespace realm {

using util::ExceptionWithBacktrace;

/// Thrown by various functions to indicate that a specified table does not
/// exist.
class NoSuchTable : public ExceptionWithBacktrace<std::exception> {
public:
    const char* message() const noexcept override;
};

class InvalidTableRef : public ExceptionWithBacktrace<std::exception> {
public:
    InvalidTableRef(const char* cause)
        : m_message(cause)
    {
    }
    const char* message() const noexcept override
    {
        return m_message.c_str();
    }
    std::string m_message;
};


/// Thrown by various functions to indicate that a specified table name is
/// already in use.
class TableNameInUse : public ExceptionWithBacktrace<std::exception> {
public:
    const char* message() const noexcept override;
};


// Thrown by functions that require a table to **not** be the target of link
// columns, unless those link columns are part of the table itself.
class CrossTableLinkTarget : public ExceptionWithBacktrace<std::exception> {
public:
    const char* message() const noexcept override;
};


/// Thrown by various functions to indicate that the dynamic type of a table
/// does not match a particular other table type (dynamic or static).
class DescriptorMismatch : public ExceptionWithBacktrace<std::exception> {
public:
    const char* message() const noexcept override;
};


/// The UnsupportedFileFormatVersion exception is thrown by DB::open()
/// constructor when opening a database that uses a deprecated file format
/// and/or a deprecated history schema which this version of Realm cannot
/// upgrade from.
class UnsupportedFileFormatVersion : public ExceptionWithBacktrace<> {
public:
    UnsupportedFileFormatVersion(int source_version);
    /// The unsupported version of the file.
    int source_version = 0;
};


/// Thrown when a sync agent attempts to join a session in which there is
/// already a sync agent. A session may only contain one sync agent at any given
/// time.
class MultipleSyncAgents : public ExceptionWithBacktrace<std::exception> {
public:
    const char* message() const noexcept override;
};


/// Thrown when memory can no longer be mapped to. When mmap/remap fails.
class AddressSpaceExhausted : public std::runtime_error {
public:
    AddressSpaceExhausted(const std::string& msg);
    /// runtime_error::what() returns the msg provided in the constructor.
};

/// Thrown when creating references that are too large to be contained in our ref_type (size_t)
class MaximumFileSizeExceeded : public std::runtime_error {
public:
    MaximumFileSizeExceeded(const std::string& msg);
    /// runtime_error::what() returns the msg provided in the constructor.
};

/// Thrown when writing fails because the disk is full.
class OutOfDiskSpace : public std::runtime_error {
public:
    OutOfDiskSpace(const std::string& msg);
    /// runtime_error::what() returns the msg provided in the constructor.
};

/// Thrown when a key can not by found
class KeyNotFound : public std::runtime_error {
public:
    KeyNotFound(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

/// Thrown when a key is already existing when trying to create a new object
class KeyAlreadyUsed : public std::runtime_error {
public:
    KeyAlreadyUsed(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

// SerialisationError intentionally does not inherit ExceptionWithBacktrace
// because the query-based-sync permissions queries generated on the server
// use a LinksToNode which is not currently serialisable (this limitation can
// be lifted in core 6 given stable ids). Coupled with query metrics which
// serialize all queries, the capturing of the stack for these frequent
// permission queries shows up in performance profiles.
class SerialisationError : public std::runtime_error {
public:
    SerialisationError(const std::string& msg);
    /// runtime_error::what() returns the msg provided in the constructor.
};

// thrown when a user constructed link path is not a valid input
class InvalidPathError : public std::runtime_error {
public:
    InvalidPathError(const std::string& msg);
    /// runtime_error::what() returns the msg provided in the constructor.
};

class DuplicatePrimaryKeyValueException : public std::logic_error {
public:
    DuplicatePrimaryKeyValueException(std::string object_type, std::string property);

    std::string const& object_type() const
    {
        return m_object_type;
    }
    std::string const& property() const
    {
        return m_property;
    }

private:
    std::string m_object_type;
    std::string m_property;
};

class NoSubscriptionForWrite : public std::runtime_error {
public:
    NoSubscriptionForWrite(const std::string& msg);
};

namespace query_parser {

/// Exception thrown when parsing fails due to invalid syntax.
struct SyntaxError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

/// Exception thrown when binding a syntactically valid query string in a
/// context where it does not make sense.
struct InvalidQueryError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

/// Exception thrown when there is a problem accessing the arguments in a query string
struct InvalidQueryArgError : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

} // namespace query_parser

/// The \c LogicError exception class is intended to be thrown only when
/// applications (or bindings) violate rules that are stated (or ought to have
/// been stated) in the documentation of the public API, and only in cases
/// where the violation could have been easily and efficiently predicted by the
/// application. In other words, this exception class is for the cases where
/// the error is due to incorrect use of the public API.
///
/// This class is not supposed to be caught by applications. It is not even
/// supposed to be considered part of the public API, and therefore the
/// documentation of the public API should **not** mention the \c LogicError
/// exception class by name. Note how this contrasts with other exception
/// classes, such as \c NoSuchTable, which are part of the public API, and are
/// supposed to be mentioned in the documentation by name. The \c LogicError
/// exception is part of Realm's private API.
///
/// In other words, the \c LogicError class should exclusively be used in
/// replacement (or in addition to) asserts (debug or not) in order to
/// guarantee program interruption, while still allowing for complete
/// test-cases to be written and run.
///
/// To this effect, the special `CHECK_LOGIC_ERROR()` macro is provided as a
/// test framework plugin to allow unit tests to check that the functions in
/// the public API do throw \c LogicError when rules are violated.
///
/// The reason behind hiding this class from the public API is to prevent users
/// from getting used to the idea that "Undefined Behaviour" equates a specific
/// exception being thrown. The whole point of properly documenting "Undefined
/// Behaviour" cases is to help the user know what the limits are, without
/// constraining the database to handle every and any use-case thrown at it.
class LogicError : public ExceptionWithBacktrace<std::exception> {
public:
    enum ErrorKind {
        string_too_big,
        binary_too_big,
        table_name_too_long,
        column_name_too_long,
        column_name_in_use,
        invalid_column_name,
        table_index_out_of_range,
        row_index_out_of_range,
        column_index_out_of_range,
        string_position_out_of_range,
        link_index_out_of_range,
        bad_version,
        illegal_type,

        /// Indicates that an argument has a value that is illegal in combination
        /// with another argument, or with the state of an involved object.
        illegal_combination,

        /// Indicates a data type mismatch, such as when `Table::find_pkey_int()` is
        /// called and the type of the primary key is not `type_Int`.
        type_mismatch,

        /// Indicates that two involved tables are not in the same group.
        group_mismatch,

        /// Indicates that an involved descriptor is of the wrong kind, i.e., if
        /// it is a subtable descriptor, and the function requires a root table
        /// descriptor.
        wrong_kind_of_descriptor,

        /// Indicates that an involved table is of the wrong kind, i.e., if it
        /// is a subtable, and the function requires a root table, or if it is a
        /// free-standing table, and the function requires a group-level table.
        wrong_kind_of_table,

        /// Indicates that an involved accessor is was detached, i.e., was not
        /// attached to an underlying object.
        detached_accessor,

        /// Indicates that a specified row index of a target table (a link) is
        /// out of range. This is used for disambiguation in cases such as
        /// Table::set_link() where one specifies both a row index of the origin
        /// table, and a row index of the target table.
        target_row_index_out_of_range,

        // Indicates that an involved column lacks a search index.
        no_search_index,

        /// Indicates that a modification was attempted that would have produced a
        /// duplicate primary value.
        unique_constraint_violation,

        /// User attempted to insert null in non-nullable column
        column_not_nullable,

        /// Group::open() is called on a group accessor that is already in the
        /// attached state. Or Group::open() or Group::commit() is called on a
        /// group accessor that is managed by a DB object.
        wrong_group_state,

        /// No active transaction on a particular Transaction object (e.g. after commit)
        /// or the Transaction object is of the wrong type (write to a read-only transaction)
        wrong_transact_state,

        /// Attempted use of a continuous transaction through a DB
        /// object with no history. See Replication::get_history().
        no_history,

        /// Durability setting (as passed to the DB constructor) was
        /// not consistent across the session.
        mixed_durability,

        /// History type (as specified by the Replication implementation passed
        /// to the DB constructor) was not consistent across the
        /// session.
        mixed_history_type,

        /// History schema version (as specified by the Replication
        /// implementation passed to the DB constructor) was not
        /// consistent across the session.
        mixed_history_schema_version,

        /// Adding rows to a table with no columns is not supported.
        table_has_no_columns,

        /// Referring to a column that has been deleted.
        column_does_not_exist,

        /// You can not add index on a subtable of a subtable
        subtable_of_subtable_index,

        /// You try to instantiate a collection object not matching column type
        collection_type_mismatch
    };

    LogicError(ErrorKind message);

    const char* message() const noexcept override;
    ErrorKind kind() const noexcept;

private:
    ErrorKind m_kind;
};


// Implementation:

// LCOV_EXCL_START (Wording of what() strings are not to be tested)

inline const char* NoSuchTable::message() const noexcept
{
    return "No such table exists";
}

inline const char* TableNameInUse::message() const noexcept
{
    return "The specified table name is already in use";
}

inline const char* CrossTableLinkTarget::message() const noexcept
{
    return "Table is target of cross-table link columns";
}

inline const char* DescriptorMismatch::message() const noexcept
{
    return "Table descriptor mismatch";
}

inline UnsupportedFileFormatVersion::UnsupportedFileFormatVersion(int version)
    : ExceptionWithBacktrace<>(
          util::format("Database has an unsupported version (%1) and cannot be upgraded", version))
    , source_version(version)
{
}

inline const char* MultipleSyncAgents::message() const noexcept
{
    return "Multiple sync agents attempted to join the same session";
}

// LCOV_EXCL_STOP

inline AddressSpaceExhausted::AddressSpaceExhausted(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline MaximumFileSizeExceeded::MaximumFileSizeExceeded(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline OutOfDiskSpace::OutOfDiskSpace(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline SerialisationError::SerialisationError(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline InvalidPathError::InvalidPathError(const std::string& msg)
    : runtime_error(msg)
{
}

inline NoSubscriptionForWrite::NoSubscriptionForWrite(const std::string& msg)
    : std::runtime_error(msg)
{
}

inline LogicError::LogicError(LogicError::ErrorKind k)
    : m_kind(k)
{
}

inline LogicError::ErrorKind LogicError::kind() const noexcept
{
    return m_kind;
}


} // namespace realm


#endif // REALM_EXCEPTIONS_HPP
