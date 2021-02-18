## Redis for Application Caching
Caching the user’s last 3 months’ transaction history
```
>> SET user-101:last-3-months-transactions “transaction-data”
OK
```

Expire a cache every 24 hours:
```
>> SET user-101:last-3-months-transactions “transaction-data” EX 86400
OK
```

Reading the cache:
```
>> GET user-101:last-3-months-transactions
```
If your application is not showing all three months’ transaction history at a time, you may further break the data in weekly and monthly transactions, and give them key names accordingly.
```
>> SET user-101:JAN-2020-transactions “JAN-2020-transaction-data” EX 86400
OK
>> SET user-101:FEB-2020-transactions “FEB-2020-transaction-data” EX 86400
OK
>> SET user-101:MARCH-2020-transactions “MARCH-2020-transaction-data” EX 86400
OK
```

## Redis for User Sessions
Basic user session for a gaming application:
```
>>HMSET user:1001 name “suyog” email “xxxx@xxx.xxx” score “3422” rank “1” auth “token”
OK
```
Once the session is created for user 1001, you can update his score or rank just using HINCRBY:
```
>>HINCRBY user:1001 score “100”
3522
```


## Redis for Leaderboards
ZADD to add one or multiple members with their scores
```
>> ZADD leaderboard 10 suyog 20 chinmay
(integer) 2
>> ZADD leaderboard 30 antirez
(integer) 11
```

To view the latest leaders, we can use the ZREVRANGE or ZRANGE commands:
```
>> ZREVRANGE leaderboard 0 -1 WITHSCORES
1) "suyog"
2) "30"
3) "antirez"
4) "30"
5) "chinmay"
6) "20"
```

Increase or decrease the specific member score
```
>> ZINCRBY leaderboard 20 suyog
"30"
```


## Redis Pub/Sub for Notifications Purpose
The following is an example where we can say that two users have initiated chatting, and they both subscribed to their respective chat rooms:
```
>> SUBSCRIBE user:100:message-to-user:200  
Reading messages... (press Ctrl-C to quit) 
1) "subscribe" 
2) "user:100:message-to-user:200" 
3) (integer) 1
```
```
>> SUBSCRIBE user:200:message-to-user:100  
Reading messages... (press Ctrl-C to quit) 
1) "subscribe" 
2) "user:200:message-to-user:100" 
3) (integer) 1 
```

Now, user 100 message to user 200:
```
>> PUBLISH user:100:message-to-user:200  "Redis is a great key value pair database"  
(integer) 1  
```


## Redis for Geospatial Data
Add a Car
```
>> GEOADD cars -115.17087 36.12306 suyogs-car
```
The first argument is the set we’re adding to, the second is the longitude, the third is the latitude, and the fourth is the member name.

To update the location of the car, you’ll just need to run the command again with new coordinates.
```
>> GEOADD cars -115.17172 36.12196 suyogs-car
```
Let’s add a second car to the “cars” set – this time, it’s driven by Robin:

>> GEOADD cars -115.171971 36.120609 robins-car

GEODIST Command to calculate distance:
```
>> GEODIST cars suyogs-car robins-car
"90.7082"
```
This means the two vehicles are about 90 meters apart. You can also specify other units:
```
>> GEODIST cars suyogs-car robins-car ft
"297.5990“
```


Now, let’s see what items are in a radius from a certain point:
```
>> GEORADIUS cars -115.17258 36.11996 100 m
1) “suyogs-car"
2) "robins-car"
```

This returns all the members within 100 meters of the point given:
```
>> GEORADIUSBYMEMBER cars robins-car 100 m
1) "robins-car"
2) “suyogs-car"
```

We can also include the distance by adding in the optional argument WITHDIST; this works for GEORADIUS or GEORADIUSBYMEMBER:
```
>> GEORADIUSBYMEMBER cars robins-car 100 m WITHDIST
1) 1) "robins-car"
   2) "0.0000"
2) 1) “suyogs-car"
   2) "90.7082"
```
```

>> GEORADIUSBYMEMBER cars robins-car 100 m WITHDIST WITHCOORD
1) 1) "robins-car"
   2) "0.0000"
   3) 1) "-115.17197102308273315"
      2) "36.12060917648089031"
2) 1) “suyogs-car"
   2) "90.7082"
   3) 1) "-115.17258256673812866"
      2) "36.11996028786411017"
```

If we want to remove “suyogs-car” from the “cars” set, we need to use the sorted set command ZREM:
```
>> ZREM cars suyogs-car
```

Deleting the Index -The geospatial index may be deleted entirely. Since the index is stored as a Redis key, Redis’ DEL command can be used to delete it:
```
>> DEL cars
```

RedisGears Use Cases
Let’s create a simple RedisGears script that will add a new user to hyperloglog also so that we can quickly check whether the user already exists:
Code for the same:


## RedisGears 
we will implement a hyperloglog that would maintain the count of all the users in the system
```python
defget_user_id(user):
    return user['value']['userId']

defadd_to_hyperloglog(user_id):
    execute('PFADD','users_exists',user_id)

gb = GearsBuilder() 
gb.map(get_user_id)     # extract user_id from the redis hash
gb.foreach(add_to_hyperloglog)  # add user_id to redishyperloglog
gb.register('users:*')  ## listen to all keys starting with users these keys denote our users
```

Now, when we add any hash in Redis whose key starts with “users:” our Redis gears script will get executed:
```
hmset users:14567 userId 14567 firstNameChinmaylastName Kulkarni
```

We will check the count now:
```
PFCOUNT user_exists
```

## Redis as Search Engine Database – RediSearch Use Cases
Let’s take an example to demonstrate the RediSearch commands for order history:
```
FT.CREATE,
OrderHistoryIndex,SCHEMA,
id,NUMERIC,SORTABLE,
customerName,TEXT,
customerCountry,TEXT,SORTABLE,
productName,TEXT,SORTABLE,
productPrice,NUMERIC,SORTABLE,
purchaseDateTime,TEXT,SORTABLE,
productDepartment,TEXT,SORTABLE,
purchaseLocation, GEO,
longitude,NUMERIC, NOINDEX,
latitude,NUMERIC, NOINDEX
```

Here’s an example of some querying order history data:
#### SIMPLE Search - search keyword across all fields
```
>>FT.SEARCHOrderHistoryIndex India
```
```
>>FT.SEARCHOrderHistoryIndex: "@customerCountry:canada& @productName:handmade" SORTBY productName DESC LIMIT 0 20
```
```
>>FT.SEARCHOrderHistoryIndex: "@customerName:%sara%" Return 2 customerNamecustomerCountry
```

##### SIMPLE Search - Sorting
```redis
>>FT.SEARCHOrderHistoryIndex"@customerCountry:india" SORTBY productName DESC LIMIT 0 2
```

##### SIMPLE Search - Geo filtering 
```
>>FT.SEARCHOrderHistoryIndex: * geofilterpurchaseLocation 72.8258 18.975 100 km
>>FT.SEARCHOrderHistoryIndex: * geofilterpurchaseLocation 74 40 100 km SORTBY purchaseLocation ASC
```
#### SIMPLE Search - Groupby
```
>>FT.AGGREGATEOrderHistoryIndex: * GROUPBY 1 @customerCountry REDUCE COUNT 0 AS ordersByCountry SORTBY 2 @ordersByCountry DESC MAX 10 LIMIT 0 10
```

#### Redis for Graphical Data Use Cases
Build a social graph using RedisGraph.
```
Create (:Person {name:'Chinmay Kulkarni'})
Create (:Person {name:'Suyog Kale'})
Create (:Person {name:'Atharva Kulkarni'})
Create (:Person {name:'Tejas'})
Create (:Person {name:'Kimaya'})
Create (:Person {name:'Prasad'})
```

Now, we query all this data using the following query to get all the people in our social network.
```
Match (p: Person) return p;
```

Implement follow functionality
```
match (a:Person),(b:Person) where a.name='Chinmay Kulkarni' AND b.name='Suyog Kale' create (a)-[r:Follow]->(b) return type(r);
```
Adding more relationships:
```
match (a:Person),(b:Person) where a.name="ChinmayKularni" and b.name="Suyog Kale" 
create (a)-[r:Follow]->(b)
return a,b,r;

match (a:Person),(b:Person) where a.name="ChinmayKularni" and b.name="Kimaya" 
create (a)-[r:Follow]->(b)
return a,b,r;

match (a:Person),(b:Person) where a.name="Atharva Kulkarni" and b.name="Kimaya" 
create (a)-[r:Follow]->(b)
return a,b,r;

match (a:Person),(b:Person) where a.name="Prasad" and b.name="Kimaya" 
create (a)-[r:Follow]->(b)
return a,b,r;

match (a:Person),(b:Person) where a.name="Tejas" and b.name="Prasad" 
create (a)-[r:Follow]->(b)
return a,b,r;

match (a:Person),(b:Person) where a.name="Suyog Kale" and b.name="Tejas" 
create (a)-[r:Follow]->(b)
return a,b,r;
```

#### Friends suggestions based on the current “following” people:

```
match (a:Person)-[r1:Follow]->(b:Person)-[r2:Follow]->(c:Person) where a.name="ChinmayKularni" return a, r1,b,r2,c
```

## Redis for Data Analytics Use Cases Using RedisTimeSeries

Let’s take a use case for implementing a smart home solution for regulation room temperature:
```
Creating Timeseries entries for a combination of properties and their room IDs:
>>TS.CREATE smarthomes:2:32 LABELS property_id 2 room_id 32
"OK"
>>TS.CREATE smarthomes:1:11 LABELS property_id 1 room_id 11
"OK"
>>TS.CREATE smarthomes:3:22 LABELS property_id 3 room_id 22
"OK"
```
Keep adding room temperatures to respective rooms:
```
>> TS.ADD smarthomes:3:22 * 31
(integer) 1599407949231

>> TS.ADD smarthomes:1:11 * 38
(integer) 1599407954981

>> TS.ADD smarthomes:2:32 * 32
(integer) 1599407958370

>> TS.ADD smarthomes:1:11 * 32
(integer) 1599407961824

>> TS.ADD smarthomes:3:22 * 34
(integer) 1599407965614

>> TS.ADD smarthomes:2:32 * 27
(integer) 1599407973649

>> TS.ADD smarthomes:1:11 * 29
(integer) 1599407977501

>> TS.ADD smarthomes:3:22 * 29
(integer) 1599407981554
```