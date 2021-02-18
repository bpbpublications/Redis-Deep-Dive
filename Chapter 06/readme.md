## Redis Strings

To set new string key or to update an existing key SET command can be used, example let's say we want generating OTP (one time password ) to let user access the web application we can use use SET to create new OTP:
```
redis> SET otp:use_10001 4532
```
Output:
"OK"

### OTP Example using Redis:
SET OTP:
```
redis> SET otp:use_10001 4532 EX 60
"OK"
```
SET Expiry
```
redis> TTL otp:use_10001
(integer) 50
```
GET OTP for matching with user provided otp:
```
redis> GET otp:use_10001
"4532"
```
```
redis> TTL otp:use_10001
(integer) -2
```
```
redis> GET otp:use_10001
(nil)
```

### Redis Hashes

Add a Bogger
```
redis> HSET blogger:10001 lastvisit "April 05, 2020 10:47:00" name "Suyog Kale" points 54210 rank 1 blogposts 45 reviews 50 followers 560
```

```
redis> HGETALL blogger:10001
```
Result:
```
1) "lastvisit"
 2) "April 05, 2020 10:47:00"
 3) "name"
 4) "Suyog Kale"
 5) "points"
 6) "54210"
 7) "rank"
 8) "1"
 9) "blogposts"
10) "45"
11) "reviews"
12) "50"
13) "followers"
14) "560"
```
Fetching specific Key:
```
redis> HMGET blogger:10001 name points blogposts
1) "Suyog Kale"
2) "54310"
3) "48"
```
Increase blogger points:
```
redis> HINCRBY blogger:10001 points 100
(integer) 54310
```

### Redis Lists

LPOP list-images to fetch one item at a time from left. 
```
>> LPOP students
```

### Redis Sorted Sets
Add User to the leaderboard leaderboard


```
redis> ZADD game1:leaderboard 100 user:101 200 user:201
(integer) 2

redis> ZADD game1:leaderboard 90 user:301
(integer) 1
```


Get user Rank from the leaderboard:
```
ZRANK record “2”
```

### Redis Hyperloglogs
Index items in Hyperloglog key 
```
PFADD class1 Noah Julie Kevin Harry Oliver
```
Count the number of elements in the set
```
PFCOUNT class1
```


### Redis Bit Arrays: Tracking user activity:
Track user login activity:

```
SETBIT userlogin:11-04-2020 101 1

```

Check if a user has logged in:
```
redis> GETBIT userlogin:11-04-2020 101
(integer) 1 

redis> GETBIT userlogin:11-04-2020 301
(integer) 0

```

Here 0 signifies that user has not logged in while 1 signifies that user has logged in.


Merge multiple keys for a month, and then get unique counts for login:
```
redis> SETBIT userlogin:11-04-2020 201 1
(integer) 0

redis> SETBIT userlogin:11-04-2020 101 1
(integer) 0

redis> SETBIT userlogin:12-04-2020 101 1
(integer) 0

redis> SETBIT userlogin:12-04-2020 301 1
(integer) 0

redis> BITOP OR userlogin:april userlogin:11-04-2020 userlogin:12-04-2020
(integer) 38

redis> BITCOUNT userlogin:april
(integer) 3

redis> GETBIT userlogin:april 101
(integer) 1

redis> GETBIT userlogin:april 201
(integer) 1

redis> GETBIT userlogin:april 301
(integer) 1

redis> GETBIT userlogin:april 401
(integer) 0

```


Redis Streams:

Inserting system logs in Redis streams:
```
redis> XADD system-log * pid 2311 cpu 25.3%
"1586599579779-0"

redis> XADD system-log * pid 1011 cpu 13.13%
"1586599632793-0"

```


Get Logs in a Range:

```
redis> XRANGE system-log - +
1) 1) "1586599579779-0"
   2) 1) "pid"
      2) "2311"
      3) "cpu"
      4) "25.3%"
2) 1) "1586599632793-0"
   2) 1) "pid"
      2) "1011"
      3) "cpu"
      4) "13.13%"

redis> XRANGE system-log 1586599579779-0 1586599579779-0
1) 1) "1586599579779-0"
   2) 1) "pid"
      2) "2311"
      3) "cpu"
      4) "25.3%"

redis> XRANGE system-log - + COUNT 1
1) 1) "1586599579779-0"
   2) 1) "pid"
      2) "2311"
      3) "cpu"
      4) "25.3%"

```


Redis Geolocation (Geo Spatial Index)

Tracking location of the cabs:
```
GEOADD pune-cabs -115.17087 36.12306 sushant-cab 
GEOADD pune-cabs -115.17087 36.12306 sushant-cab
```

Find distance between Customer and Cab:
```
GEORADIUS pune-cabs -115.15258 36.12996 100 m
```