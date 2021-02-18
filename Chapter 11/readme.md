### ACLS

ACL list
```
>> ACL list
1) "user default on ~* +@all"
```

Create Admin user with access to all commands and keys:
```
ACL SETUSER admin on >redispass ~* -@all +@admin
```

View specific user permission details
```
ACL GETUSER default
```

User suyog who has access to keys starting with user: and can only GET -> read these keys but cannot write them:
```
ACL SETUSER suyog on >suyogpass ~user:* +get 
```


## Client Side Caching
Enable Tracking
```
CLIENT TRACKING on REDIRECT 345 OPTIN
```
Tell Redis Server Redis Client wants to cache
```
CLIENT CACHING YES
```

### Redis Threading
```
redis-benchmark --threads 4 -t get -n 10000000
```