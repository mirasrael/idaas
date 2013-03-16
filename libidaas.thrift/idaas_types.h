/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef idaas_TYPES_H
#define idaas_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace idaas {

typedef struct _ida_enum_const__isset {
  _ida_enum_const__isset() : mask(true) {}
  bool mask;
} _ida_enum_const__isset;

class ida_enum_const {
 public:

  static const char* ascii_fingerprint; // = "28C2ECC89260BADB9C70330FBF47BFA8";
  static const uint8_t binary_fingerprint[16]; // = {0x28,0xC2,0xEC,0xC8,0x92,0x60,0xBA,0xDB,0x9C,0x70,0x33,0x0F,0xBF,0x47,0xBF,0xA8};

  ida_enum_const() : name(), value(0), mask(-1) {
  }

  virtual ~ida_enum_const() throw() {}

  std::string name;
  int32_t value;
  int32_t mask;

  _ida_enum_const__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_value(const int32_t val) {
    value = val;
  }

  void __set_mask(const int32_t val) {
    mask = val;
  }

  bool operator == (const ida_enum_const & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(value == rhs.value))
      return false;
    if (!(mask == rhs.mask))
      return false;
    return true;
  }
  bool operator != (const ida_enum_const &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ida_enum_const & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ida_enum_const &a, ida_enum_const &b);

typedef struct _ida_enum__isset {
  _ida_enum__isset() : isBitfield(true), constants(false) {}
  bool isBitfield;
  bool constants;
} _ida_enum__isset;

class ida_enum {
 public:

  static const char* ascii_fingerprint; // = "5826940A67BB88BC946A74206572D8CB";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x26,0x94,0x0A,0x67,0xBB,0x88,0xBC,0x94,0x6A,0x74,0x20,0x65,0x72,0xD8,0xCB};

  ida_enum() : name(), isBitfield(false) {
  }

  virtual ~ida_enum() throw() {}

  std::string name;
  bool isBitfield;
  std::vector<ida_enum_const>  constants;

  _ida_enum__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_isBitfield(const bool val) {
    isBitfield = val;
  }

  void __set_constants(const std::vector<ida_enum_const> & val) {
    constants = val;
  }

  bool operator == (const ida_enum & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(isBitfield == rhs.isBitfield))
      return false;
    if (!(constants == rhs.constants))
      return false;
    return true;
  }
  bool operator != (const ida_enum &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ida_enum & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ida_enum &a, ida_enum &b);


class ida_struct_member {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  ida_struct_member() : name(), type() {
  }

  virtual ~ida_struct_member() throw() {}

  std::string name;
  std::string type;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_type(const std::string& val) {
    type = val;
  }

  bool operator == (const ida_struct_member & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(type == rhs.type))
      return false;
    return true;
  }
  bool operator != (const ida_struct_member &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ida_struct_member & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ida_struct_member &a, ida_struct_member &b);

typedef struct _ida_struct__isset {
  _ida_struct__isset() : isUnion(true), members(false) {}
  bool isUnion;
  bool members;
} _ida_struct__isset;

class ida_struct {
 public:

  static const char* ascii_fingerprint; // = "F83089BCC3F27DED4B671B75FE652BE0";
  static const uint8_t binary_fingerprint[16]; // = {0xF8,0x30,0x89,0xBC,0xC3,0xF2,0x7D,0xED,0x4B,0x67,0x1B,0x75,0xFE,0x65,0x2B,0xE0};

  ida_struct() : name(), isUnion(false) {
  }

  virtual ~ida_struct() throw() {}

  std::string name;
  bool isUnion;
  std::vector<ida_struct_member>  members;

  _ida_struct__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_isUnion(const bool val) {
    isUnion = val;
  }

  void __set_members(const std::vector<ida_struct_member> & val) {
    members = val;
  }

  bool operator == (const ida_struct & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(isUnion == rhs.isUnion))
      return false;
    if (!(members == rhs.members))
      return false;
    return true;
  }
  bool operator != (const ida_struct &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ida_struct & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ida_struct &a, ida_struct &b);

} // namespace

#endif
