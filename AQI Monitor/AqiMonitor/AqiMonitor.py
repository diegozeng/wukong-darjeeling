#!/usr/bin/env python
#
# Copyright 2009 Facebook
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.

import json
import os.path
import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web

from pymongo import MongoClient

from tornado.options import define, options

define("port", default=8888, help="run on the given port", type=int)

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("index.html");


class AjaxHandler(tornado.web.RequestHandler):
    def get(self):
        client = MongoClient('localhost', 27017)
        db = client.wukong
        coll = db.readings
        
        MongoList1 = list(coll.find().sort('_id',-1).limit(1))
        del MongoList1[0]["_id"]
	del MongoList1[0]["node_id"]
	del MongoList1[0]["wuclass_id"]
	del MongoList1[0]["port"]
	del MongoList1[0]["timestamp"]
        newAqi1 = json.dumps(MongoList1[0])

        self.write(newAqi1);
    
def main():
    tornado.options.parse_command_line()
    application = tornado.web.Application([
        (r"/", MainHandler),(r"/ajax", AjaxHandler)
    ],  static_path=os.path.join(os.path.dirname(__file__), "static"),
        template_path=os.path.join(os.path.dirname(__file__), "templates"))
    
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()


if __name__ == "__main__":
    main()
