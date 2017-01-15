
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <imtjson/json.h>
#include <imtjson/path.h>
#include <imtjson/validator.h>
#include <unistd.h>


using namespace json;


std::map<String, var> storedDocs;
std::vector<var> views;

var doResetCommand(const var &cmd) {
	views.clear();
	return true;
}


static void log(var v) {
	var x = {"log",v.toString()};
	x.toStream(std::cout);
	std::endl(std::cout);

}

String formatErrorMsg(const var &rejs) {
	std::ostringstream out;

	for (var r : rejs) {

		out << "doc";
		for (var p : r[0]) {
			if (p.type() == json::number) {
				out << "[" << p.getUInt() << "]";
			} else {
				out << "." << p.getString();
			}
		}
		out << " : " << r[1].toString() << ". ";
		out << std::endl;

	}

	return String(out.str());


}

var doValidation(const var &ddoc, const var &data) {
	Validator v(ddoc["validate_doc_update"]);

	v.setVariables(Object("old", data[1])
			             ("user",data[2])
			             ("security",data[3])
			         );

	bool res = v.validate(data[0]);
	if (res) return 1;
	else return Object("forbidden", formatErrorMsg(v.getRejections()));
}

var doFilter(const var &ddoc, const String &name, const var &docs) {

	Validator v(ddoc["filters"][name]);
	Array results;
	for (Value doc: docs) {
		results.push_back(v.validate(doc));
	}
	return {true, results};
}


var doCommandDDoc(const var &cmd) {
	String id = cmd[1];
	if (id == "new") {
		String id = cmd[2];
		var doc = cmd[3];
		if (id.defined() && doc.defined()) {
			storedDocs[id] = doc;
			return true;
		}
		else return {"error","Internal error","Failed to update design document"};
	} else {

		auto iter = storedDocs.find(id);
		if (iter == storedDocs.end()) {
			return {"error","Internal error","Unknown design document"};
		}
		String subcmd = cmd[2][0];
		if (subcmd =="filters") {
			String name = cmd[2][1];
			return doFilter(iter->second, name, cmd[3]);
		} else if (subcmd == "validate_doc_update") {
			return doValidation(iter->second, cmd[3]);
		} else {
			return {"error","Unsupported","Unsupported feature"};
		}

	}

}


var doAddFun(const var &cmd) {
	views.push_back(var::fromString(cmd[1].getString()));
	return true;
}

var doMapDoc(const var &cmd) {

	Array r;
	Array o;
	for (var x : views) {
		Validator v(x);
		bool res = v.validate(cmd[1]);
		if (res) {
			var emits = v.getEmits();
			if (emits.empty()) {
				r.add({{nullptr,nullptr}});
			} else {
				o.clear();
				for (var z:emits) {
					if (z.type() == array && z.size() == 2) {
						o.add(z);
					} else {
						o.add({nullptr, z});
					}
				}
				r.add(o);
			}
		} else{
			r.add(array);
		}
	}
	return r;
}


int main(void) {

	try {
	do {


		var v = Value::fromStream(std::cin);
//		log(v);
		var res;
		try {

			String cmd = v[0];
			if (cmd == "reset") res = doResetCommand(v);
			else if (cmd == "ddoc") res=doCommandDDoc(v);
			else if (cmd == "add_fun") res=doAddFun(v);
			else if (cmd == "map_doc") res = doMapDoc(v);
			else res = {"error","unsupported","Operation is not supported by this query server"};

		} catch (std::exception &e) {
			res = {"error", "exception",e.what() };
		}
		res.toStream(std::cout);
		std::cout << std::endl;

	} while (true);
	} catch (std::exception &e) {
		std::cout << var({"error", e.what() }).stringify() << std::endl;
	}

}
