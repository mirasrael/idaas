#include "struct_copiers.h"

#pragma warning(disable: 4996)
#include <typeinf.hpp>
#include <enum.hpp>
#pragma warning(default: 4996)

#include <boost/lexical_cast.hpp>

void struct_list_copier::copy_to(std::vector<IdaStruct>&  _result) {
	size_t count = get_struc_qty();
	_result.resize(count);
	for (size_t idx = 0; idx < count; idx++) {
		tid_t id = get_struc_by_idx(idx);		
		struct_copier::copy(get_struc(id), _result.at(idx));
	}
}

void struct_copier::copy(const struc_t *from, IdaStruct& to) {
	char buffer[4096];
	get_struc_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;
	to.isUnion = from->is_union();

	size_t membersCount = from->memqty;
	to.members.reserve(membersCount * 2);
	to.members.resize(membersCount);
	ea_t prev_end = 0;
	for (size_t memberIdx = 0, targetIndex = 0; memberIdx < membersCount; memberIdx++, targetIndex++) {
		member_t *member = &from->members[memberIdx];
		if (member->soff > prev_end) {
			to.members.resize(to.members.size() + 1);
			IdaStructMember& targetMember = to.members[targetIndex];
			targetMember.name = std::string("__gap__") + boost::lexical_cast<std::string>(targetIndex);
			targetMember.type = std::string("unsigned char[") + boost::lexical_cast<std::string>(member->soff - prev_end) + std::string("]");
			targetIndex++;
		}
		struct_member_copier::copy(member, to.members[targetIndex]);
		prev_end = member->eoff;
	}	
}

void struct_member_copier::copy(const member_t *from, IdaStructMember& to) {
	char buffer[4096];
	get_member_name(from->id, buffer, sizeof(buffer));
	to.name = buffer;
	qtype type, fields;
	if (from->has_ti()) {
		get_member_tinfo(from, &type, &fields);
	} else {
		if (0 != (enumflag() & from->flag)) {
			opinfo_t opinfo;
			qstring name;
			retrieve_member_info(from, &opinfo);
			get_enum_name(opinfo.ec.tid, buffer, sizeof(buffer));
			qstrncat(buffer, ";", sizeof(buffer));			
			parse_decl(idati, buffer, &name, &type, &fields, 0);
			size_t width = get_enum_width(opinfo.ec.tid);
			width = width > 0 ? 1 << (width - 1) : sizeof(int);
			if (from->eoff - from->soff > width) {
				qtype elementType = type;
				build_array_type(&type, elementType.c_str(), (from->eoff - from->soff) / width);
			}
		} else {
			get_or_guess_member_tinfo(from, &type, &fields);
		}		
	}	
	print_type_to_one_line(buffer, sizeof(buffer), idati, type.c_str(), 0, 0, fields.c_str(), 0);
	to.type = buffer;
}
