#!/usr/bin/python
#python3
import os

cnt = 0
top = os.getcwd()
for root, dirs, files in os.walk(top, topdown=False):
    for name in files:
        #print(os.path.join(root, name))
        if 'ﬁ' in name:
            os.rename(os.path.join(root, name), os.path.join(root, name.replace('ﬁ', 'fi')))
            cnt += 1
print(cnt, "file(s) changed!")
