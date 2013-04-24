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

struct IdaRefType {
  enum type {
    Data = 1
  };
};

extern const std::map<int, const char*> _IdaRefType_VALUES_TO_NAMES;

struct IdaOperandType {
  enum type {
    Unknown = 0,
    Register = 1,
    Constant = 2,
    Memory = 3,
    Dislacement = 4,
    Address = 5,
    FPRegister = 6
  };
};

extern const std::map<int, const char*> _IdaOperandType_VALUES_TO_NAMES;

struct IdaRegister {
  enum type {
    Unknown = 0,
    Al = 1,
    Ah = 2,
    Ax = 3,
    Eax = 4,
    Bl = 5,
    Bh = 6,
    Bx = 7,
    Ebx = 8,
    Cl = 9,
    Ch = 10,
    Cx = 11,
    Ecx = 12,
    Dl = 13,
    Dh = 14,
    Dx = 15,
    Edx = 16,
    Esi = 17,
    Edi = 18,
    Ebp = 19,
    Esp = 20,
    Xmm0 = 21,
    Xmm1 = 22,
    Xmm2 = 23,
    Xmm3 = 24,
    None = 25
  };
};

extern const std::map<int, const char*> _IdaRegister_VALUES_TO_NAMES;

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


class ida_string {
 public:

  static const char* ascii_fingerprint; // = "3F5FC93B338687BC7235B1AB103F47B3";
  static const uint8_t binary_fingerprint[16]; // = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

  ida_string() : address(0), value() {
  }

  virtual ~ida_string() throw() {}

  int32_t address;
  std::string value;

  void __set_address(const int32_t val) {
    address = val;
  }

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const ida_string & rhs) const
  {
    if (!(address == rhs.address))
      return false;
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const ida_string &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ida_string & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ida_string &a, ida_string &b);


class IdaRef {
 public:

  static const char* ascii_fingerprint; // = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
  static const uint8_t binary_fingerprint[16]; // = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};

  IdaRef() : refFrom(0), refTo(0) {
  }

  virtual ~IdaRef() throw() {}

  int32_t refFrom;
  int32_t refTo;

  void __set_refFrom(const int32_t val) {
    refFrom = val;
  }

  void __set_refTo(const int32_t val) {
    refTo = val;
  }

  bool operator == (const IdaRef & rhs) const
  {
    if (!(refFrom == rhs.refFrom))
      return false;
    if (!(refTo == rhs.refTo))
      return false;
    return true;
  }
  bool operator != (const IdaRef &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IdaRef & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(IdaRef &a, IdaRef &b);


class IdaFunction {
 public:

  static const char* ascii_fingerprint; // = "9A907BD0092CBBA78E0613C3C8249BD3";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x90,0x7B,0xD0,0x09,0x2C,0xBB,0xA7,0x8E,0x06,0x13,0xC3,0xC8,0x24,0x9B,0xD3};

  IdaFunction() : startAddress(0), endAddress(0), name(), type() {
  }

  virtual ~IdaFunction() throw() {}

  int32_t startAddress;
  int32_t endAddress;
  std::string name;
  std::string type;

  void __set_startAddress(const int32_t val) {
    startAddress = val;
  }

  void __set_endAddress(const int32_t val) {
    endAddress = val;
  }

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_type(const std::string& val) {
    type = val;
  }

  bool operator == (const IdaFunction & rhs) const
  {
    if (!(startAddress == rhs.startAddress))
      return false;
    if (!(endAddress == rhs.endAddress))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(type == rhs.type))
      return false;
    return true;
  }
  bool operator != (const IdaFunction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IdaFunction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(IdaFunction &a, IdaFunction &b);

typedef struct _IdaOperand__isset {
  _IdaOperand__isset() : register_(false), address(false), baseRegister(false), indexRegister(false), indexScale(false), value(false) {}
  bool register_;
  bool address;
  bool baseRegister;
  bool indexRegister;
  bool indexScale;
  bool value;
} _IdaOperand__isset;

class IdaOperand {
 public:

  static const char* ascii_fingerprint; // = "B4635C14DC753231940A1B762E16393A";
  static const uint8_t binary_fingerprint[16]; // = {0xB4,0x63,0x5C,0x14,0xDC,0x75,0x32,0x31,0x94,0x0A,0x1B,0x76,0x2E,0x16,0x39,0x3A};

  IdaOperand() : type((IdaOperandType::type)0), register_((IdaRegister::type)0), address(0), baseRegister((IdaRegister::type)0), indexRegister((IdaRegister::type)0), indexScale(0), size(0), value(0) {
  }

  virtual ~IdaOperand() throw() {}

  IdaOperandType::type type;
  IdaRegister::type register_;
  int32_t address;
  IdaRegister::type baseRegister;
  IdaRegister::type indexRegister;
  int32_t indexScale;
  int8_t size;
  int32_t value;

  _IdaOperand__isset __isset;

  void __set_type(const IdaOperandType::type val) {
    type = val;
  }

  void __set_register_(const IdaRegister::type val) {
    register_ = val;
  }

  void __set_address(const int32_t val) {
    address = val;
  }

  void __set_baseRegister(const IdaRegister::type val) {
    baseRegister = val;
  }

  void __set_indexRegister(const IdaRegister::type val) {
    indexRegister = val;
  }

  void __set_indexScale(const int32_t val) {
    indexScale = val;
  }

  void __set_size(const int8_t val) {
    size = val;
  }

  void __set_value(const int32_t val) {
    value = val;
  }

  bool operator == (const IdaOperand & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (!(register_ == rhs.register_))
      return false;
    if (!(address == rhs.address))
      return false;
    if (!(baseRegister == rhs.baseRegister))
      return false;
    if (!(indexRegister == rhs.indexRegister))
      return false;
    if (!(indexScale == rhs.indexScale))
      return false;
    if (!(size == rhs.size))
      return false;
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const IdaOperand &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IdaOperand & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(IdaOperand &a, IdaOperand &b);


class IdaInstruction {
 public:

  static const char* ascii_fingerprint; // = "62E54BCE50BE8DE9ED7359AB71B4C0C0";
  static const uint8_t binary_fingerprint[16]; // = {0x62,0xE5,0x4B,0xCE,0x50,0xBE,0x8D,0xE9,0xED,0x73,0x59,0xAB,0x71,0xB4,0xC0,0xC0};

  IdaInstruction() : address(0), size(0), mnemonic() {
  }

  virtual ~IdaInstruction() throw() {}

  int32_t address;
  int32_t size;
  std::string mnemonic;
  std::vector<IdaOperand>  operands;
  std::vector<std::string>  prefixes;

  void __set_address(const int32_t val) {
    address = val;
  }

  void __set_size(const int32_t val) {
    size = val;
  }

  void __set_mnemonic(const std::string& val) {
    mnemonic = val;
  }

  void __set_operands(const std::vector<IdaOperand> & val) {
    operands = val;
  }

  void __set_prefixes(const std::vector<std::string> & val) {
    prefixes = val;
  }

  bool operator == (const IdaInstruction & rhs) const
  {
    if (!(address == rhs.address))
      return false;
    if (!(size == rhs.size))
      return false;
    if (!(mnemonic == rhs.mnemonic))
      return false;
    if (!(operands == rhs.operands))
      return false;
    if (!(prefixes == rhs.prefixes))
      return false;
    return true;
  }
  bool operator != (const IdaInstruction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IdaInstruction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(IdaInstruction &a, IdaInstruction &b);

} // namespace

#endif
