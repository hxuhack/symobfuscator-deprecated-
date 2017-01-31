#!/bin/bash

git add .
git commit -m "update"
spawn git push origin master
expect "Username for 'https://github.com':"
send "hxuhack\r"
expect "Password for 'https://github.com':"
send "yurong0429\r"
