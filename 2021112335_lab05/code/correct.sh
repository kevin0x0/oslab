#/bin/sh

cat process.log.header > process.log.post
grep -E "^[^0]" process.log >> process.log.post
