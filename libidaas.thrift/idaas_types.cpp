/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "idaas_types.h"

#include <algorithm>

namespace idaas {

int _kIdaRefTypeValues[] = {
  IdaRefType::Data
};
const char* _kIdaRefTypeNames[] = {
  "Data"
};
const std::map<int, const char*> _IdaRefType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(1, _kIdaRefTypeValues, _kIdaRefTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kIdaOperandTypeValues[] = {
  IdaOperandType::Unknown,
  IdaOperandType::Register,
  IdaOperandType::Constant,
  IdaOperandType::Memory,
  IdaOperandType::Dislacement,
  IdaOperandType::Address,
  IdaOperandType::FPRegister
};
const char* _kIdaOperandTypeNames[] = {
  "Unknown",
  "Register",
  "Constant",
  "Memory",
  "Dislacement",
  "Address",
  "FPRegister"
};
const std::map<int, const char*> _IdaOperandType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(7, _kIdaOperandTypeValues, _kIdaOperandTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kIdaRegisterValues[] = {
  IdaRegister::Unknown,
  IdaRegister::Al,
  IdaRegister::Ah,
  IdaRegister::Ax,
  IdaRegister::Eax,
  IdaRegister::Bl,
  IdaRegister::Bh,
  IdaRegister::Bx,
  IdaRegister::Ebx,
  IdaRegister::Cl,
  IdaRegister::Ch,
  IdaRegister::Cx,
  IdaRegister::Ecx,
  IdaRegister::Dl,
  IdaRegister::Dh,
  IdaRegister::Dx,
  IdaRegister::Edx,
  IdaRegister::Esi,
  IdaRegister::Edi,
  IdaRegister::Ebp,
  IdaRegister::Esp,
  IdaRegister::Xmm0,
  IdaRegister::Xmm1,
  IdaRegister::Xmm2,
  IdaRegister::Xmm3,
  IdaRegister::None
};
const char* _kIdaRegisterNames[] = {
  "Unknown",
  "Al",
  "Ah",
  "Ax",
  "Eax",
  "Bl",
  "Bh",
  "Bx",
  "Ebx",
  "Cl",
  "Ch",
  "Cx",
  "Ecx",
  "Dl",
  "Dh",
  "Dx",
  "Edx",
  "Esi",
  "Edi",
  "Ebp",
  "Esp",
  "Xmm0",
  "Xmm1",
  "Xmm2",
  "Xmm3",
  "None"
};
const std::map<int, const char*> _IdaRegister_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(26, _kIdaRegisterValues, _kIdaRegisterNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* ida_enum_const::ascii_fingerprint = "28C2ECC89260BADB9C70330FBF47BFA8";
const uint8_t ida_enum_const::binary_fingerprint[16] = {0x28,0xC2,0xEC,0xC8,0x92,0x60,0xBA,0xDB,0x9C,0x70,0x33,0x0F,0xBF,0x47,0xBF,0xA8};

uint32_t ida_enum_const::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;
  bool isset_value = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->mask);
          this->__isset.mask = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ida_enum_const::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ida_enum_const");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("mask", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->mask);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ida_enum_const &a, ida_enum_const &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.value, b.value);
  swap(a.mask, b.mask);
  swap(a.__isset, b.__isset);
}

const char* ida_enum::ascii_fingerprint = "5826940A67BB88BC946A74206572D8CB";
const uint8_t ida_enum::binary_fingerprint[16] = {0x58,0x26,0x94,0x0A,0x67,0xBB,0x88,0xBC,0x94,0x6A,0x74,0x20,0x65,0x72,0xD8,0xCB};

uint32_t ida_enum::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->isBitfield);
          this->__isset.isBitfield = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->constants.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->constants.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->constants[_i4].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.constants = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ida_enum::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ida_enum");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("isBitfield", ::apache::thrift::protocol::T_BOOL, 2);
  xfer += oprot->writeBool(this->isBitfield);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("constants", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->constants.size()));
    std::vector<ida_enum_const> ::const_iterator _iter5;
    for (_iter5 = this->constants.begin(); _iter5 != this->constants.end(); ++_iter5)
    {
      xfer += (*_iter5).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ida_enum &a, ida_enum &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.isBitfield, b.isBitfield);
  swap(a.constants, b.constants);
  swap(a.__isset, b.__isset);
}

const char* ida_struct_member::ascii_fingerprint = "07A9615F837F7D0A952B595DD3020972";
const uint8_t ida_struct_member::binary_fingerprint[16] = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

uint32_t ida_struct_member::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;
  bool isset_type = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->type);
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ida_struct_member::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ida_struct_member");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ida_struct_member &a, ida_struct_member &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.type, b.type);
}

const char* ida_struct::ascii_fingerprint = "F83089BCC3F27DED4B671B75FE652BE0";
const uint8_t ida_struct::binary_fingerprint[16] = {0xF8,0x30,0x89,0xBC,0xC3,0xF2,0x7D,0xED,0x4B,0x67,0x1B,0x75,0xFE,0x65,0x2B,0xE0};

uint32_t ida_struct::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->isUnion);
          this->__isset.isUnion = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->members.clear();
            uint32_t _size6;
            ::apache::thrift::protocol::TType _etype9;
            xfer += iprot->readListBegin(_etype9, _size6);
            this->members.resize(_size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              xfer += this->members[_i10].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.members = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ida_struct::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ida_struct");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("isUnion", ::apache::thrift::protocol::T_BOOL, 2);
  xfer += oprot->writeBool(this->isUnion);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("members", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->members.size()));
    std::vector<ida_struct_member> ::const_iterator _iter11;
    for (_iter11 = this->members.begin(); _iter11 != this->members.end(); ++_iter11)
    {
      xfer += (*_iter11).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ida_struct &a, ida_struct &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.isUnion, b.isUnion);
  swap(a.members, b.members);
  swap(a.__isset, b.__isset);
}

const char* ida_string::ascii_fingerprint = "3F5FC93B338687BC7235B1AB103F47B3";
const uint8_t ida_string::binary_fingerprint[16] = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

uint32_t ida_string::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_address = false;
  bool isset_value = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->address);
          isset_address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_address)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ida_string::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ida_string");

  xfer += oprot->writeFieldBegin("address", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ida_string &a, ida_string &b) {
  using ::std::swap;
  swap(a.address, b.address);
  swap(a.value, b.value);
}

const char* IdaRef::ascii_fingerprint = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
const uint8_t IdaRef::binary_fingerprint[16] = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};

uint32_t IdaRef::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_refFrom = false;
  bool isset_refTo = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->refFrom);
          isset_refFrom = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->refTo);
          isset_refTo = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_refFrom)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_refTo)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IdaRef::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("IdaRef");

  xfer += oprot->writeFieldBegin("refFrom", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->refFrom);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("refTo", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->refTo);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IdaRef &a, IdaRef &b) {
  using ::std::swap;
  swap(a.refFrom, b.refFrom);
  swap(a.refTo, b.refTo);
}

const char* IdaFunction::ascii_fingerprint = "9A907BD0092CBBA78E0613C3C8249BD3";
const uint8_t IdaFunction::binary_fingerprint[16] = {0x9A,0x90,0x7B,0xD0,0x09,0x2C,0xBB,0xA7,0x8E,0x06,0x13,0xC3,0xC8,0x24,0x9B,0xD3};

uint32_t IdaFunction::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_startAddress = false;
  bool isset_endAddress = false;
  bool isset_name = false;
  bool isset_type = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->startAddress);
          isset_startAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->endAddress);
          isset_endAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->type);
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_startAddress)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_endAddress)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IdaFunction::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("IdaFunction");

  xfer += oprot->writeFieldBegin("startAddress", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->startAddress);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("endAddress", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->endAddress);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IdaFunction &a, IdaFunction &b) {
  using ::std::swap;
  swap(a.startAddress, b.startAddress);
  swap(a.endAddress, b.endAddress);
  swap(a.name, b.name);
  swap(a.type, b.type);
}

const char* IdaOperand::ascii_fingerprint = "B4635C14DC753231940A1B762E16393A";
const uint8_t IdaOperand::binary_fingerprint[16] = {0xB4,0x63,0x5C,0x14,0xDC,0x75,0x32,0x31,0x94,0x0A,0x1B,0x76,0x2E,0x16,0x39,0x3A};

uint32_t IdaOperand::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_type = false;
  bool isset_size = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast12;
          xfer += iprot->readI32(ecast12);
          this->type = (IdaOperandType::type)ecast12;
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast13;
          xfer += iprot->readI32(ecast13);
          this->register_ = (IdaRegister::type)ecast13;
          this->__isset.register_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->address);
          this->__isset.address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast14;
          xfer += iprot->readI32(ecast14);
          this->baseRegister = (IdaRegister::type)ecast14;
          this->__isset.baseRegister = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast15;
          xfer += iprot->readI32(ecast15);
          this->indexRegister = (IdaRegister::type)ecast15;
          this->__isset.indexRegister = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->indexScale);
          this->__isset.indexScale = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_BYTE) {
          xfer += iprot->readByte(this->size);
          isset_size = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->value);
          this->__isset.value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_size)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IdaOperand::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("IdaOperand");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("register_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((int32_t)this->register_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("address", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("baseRegister", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32((int32_t)this->baseRegister);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("indexRegister", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32((int32_t)this->indexRegister);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("indexScale", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->indexScale);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("size", ::apache::thrift::protocol::T_BYTE, 7);
  xfer += oprot->writeByte(this->size);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_I32, 8);
  xfer += oprot->writeI32(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IdaOperand &a, IdaOperand &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.register_, b.register_);
  swap(a.address, b.address);
  swap(a.baseRegister, b.baseRegister);
  swap(a.indexRegister, b.indexRegister);
  swap(a.indexScale, b.indexScale);
  swap(a.size, b.size);
  swap(a.value, b.value);
  swap(a.__isset, b.__isset);
}

const char* IdaInstruction::ascii_fingerprint = "62E54BCE50BE8DE9ED7359AB71B4C0C0";
const uint8_t IdaInstruction::binary_fingerprint[16] = {0x62,0xE5,0x4B,0xCE,0x50,0xBE,0x8D,0xE9,0xED,0x73,0x59,0xAB,0x71,0xB4,0xC0,0xC0};

uint32_t IdaInstruction::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_address = false;
  bool isset_size = false;
  bool isset_mnemonic = false;
  bool isset_operands = false;
  bool isset_prefixes = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->address);
          isset_address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->size);
          isset_size = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->mnemonic);
          isset_mnemonic = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->operands.clear();
            uint32_t _size16;
            ::apache::thrift::protocol::TType _etype19;
            xfer += iprot->readListBegin(_etype19, _size16);
            this->operands.resize(_size16);
            uint32_t _i20;
            for (_i20 = 0; _i20 < _size16; ++_i20)
            {
              xfer += this->operands[_i20].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_operands = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->prefixes.clear();
            uint32_t _size21;
            ::apache::thrift::protocol::TType _etype24;
            xfer += iprot->readListBegin(_etype24, _size21);
            this->prefixes.resize(_size21);
            uint32_t _i25;
            for (_i25 = 0; _i25 < _size21; ++_i25)
            {
              xfer += iprot->readString(this->prefixes[_i25]);
            }
            xfer += iprot->readListEnd();
          }
          isset_prefixes = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_address)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_size)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_mnemonic)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_operands)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_prefixes)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t IdaInstruction::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("IdaInstruction");

  xfer += oprot->writeFieldBegin("address", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("size", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->size);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("mnemonic", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->mnemonic);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("operands", ::apache::thrift::protocol::T_LIST, 4);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->operands.size()));
    std::vector<IdaOperand> ::const_iterator _iter26;
    for (_iter26 = this->operands.begin(); _iter26 != this->operands.end(); ++_iter26)
    {
      xfer += (*_iter26).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("prefixes", ::apache::thrift::protocol::T_LIST, 5);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->prefixes.size()));
    std::vector<std::string> ::const_iterator _iter27;
    for (_iter27 = this->prefixes.begin(); _iter27 != this->prefixes.end(); ++_iter27)
    {
      xfer += oprot->writeString((*_iter27));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IdaInstruction &a, IdaInstruction &b) {
  using ::std::swap;
  swap(a.address, b.address);
  swap(a.size, b.size);
  swap(a.mnemonic, b.mnemonic);
  swap(a.operands, b.operands);
  swap(a.prefixes, b.prefixes);
}

const char* IdaTypeInfo::ascii_fingerprint = "AB879940BD15B6B25691265F7384B271";
const uint8_t IdaTypeInfo::binary_fingerprint[16] = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

uint32_t IdaTypeInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          this->__isset.name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->type);
          this->__isset.type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->fields);
          this->__isset.fields = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t IdaTypeInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("IdaTypeInfo");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("fields", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->fields);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IdaTypeInfo &a, IdaTypeInfo &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.type, b.type);
  swap(a.fields, b.fields);
  swap(a.__isset, b.__isset);
}

} // namespace
