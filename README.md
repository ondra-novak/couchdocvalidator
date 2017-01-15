# couchdocvalidator
A QueryServer for CouchDB which intended to validate or filter documents defined by a schema, which has been introduced with the imtjson library

It requires imtjson to compile. The library must be installed in the system.

Use cmake to build and install.

After installation, the language "jSchema" appear. You can use "jSchema" for views, filters and validation, other functions
are not supported. Fot the view, the schema must be serialized into the string. For the filters and validation, no serialization is required
