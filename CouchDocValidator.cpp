//============================================================================
// Name        : CouchDocValidator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <imtjson/json.h>
#include <imtjson/path.h>


using namespace json;

var doResetCommand(const var &cmd) {
	return true;
}

std::map<String, var> storedDocs;

bool updateDDoc(String id, var document) {


}


var doCommandDDoc(const var &cmd) {
	String id = cmd[2];
	if (id == "new") {
		String id = cmd[3];
		var doc = cmd[4];
		if (id.defined() && doc.defined()) {
			storedDocs[id] = doc;
			return true;
		}
		else return {"error","Internal error","Failed to update design document"};
	} else {
		auto iter = storedDocs.find(id);
		if (iter == storedDocs.end()) {
			return {"error","Internal error","Unknown design document"};
		String subcmd = doc[3][0];
		if (subcmd =="filter") {

		} else if (subcmd == "validate_doc_update") {

		} else {
			return {"error","Unsupported","Unsupported feature"};
		}

	}

}

int main(void) {

do {

	var v = Value::fromStream(std::cin);
	var res;
	try {

		String cmd = res[0];
		if (cmd == "reset") res = doResetCommand(v);
		else if (cmd == "ddoc") res=doCommandDDoc(v);
		else res = {"error","unsupported","Operation is not supported by this query server"};

	} catch (std::exception &e) {
		res = {"error", e.what() };
	}
	res.toStream(std::cout);
	std::cout << std::endl;

} while (true);

}
