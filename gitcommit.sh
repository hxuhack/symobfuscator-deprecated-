#!/bin/bash
#!/usr/bin/expect

git add .
git commit -m "update"
git push origin master
expect "Username for 'https://github.com':"
send "hxuhack\r"
expect "Password for 'https://github.com':"
send "yurong0429\r"
