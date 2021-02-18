## Loading a Redis Module:
```
loadmodule /path/to/mymodule.so
```

### Installing RedisJSON
RedisJSON uses the new Rust interface for creating modules, so we will have to setup the same.
```
git clone https://github.com/RedisJSON/RedisJSON.git
apt-get install build-essential
curl https://sh.rustup.rs -sSf | sudosh 
brew install cmake
make
redis-server --loadmodule /path/to/module/rejson.so
```


## Redis - Writing a module in Redis

Description - We will create a multiplication module in Redis

*Steps to Install*

Step 1 - Compile the code
- Execute following command to compile (install gcc compiler if not exists)
- gcc -fPIC -shared -std=gnu99 -o maths_module.o maths_module.c

Step 2 - Include Module to redis (we will use redis-cli, there are other was to to include modules)
- Make sure you redis server is running (>= redis 5.x)
- Go to redis-cli
- Execute MODULE LOAD /path/to/maths_module.o

Step 3 - Test module
- Go to redis-cli and execute your command
127.0.0.1:6379> maths.add 1 4
(integer) 5
127.0.0.1:6379> 

Example Code:
```
Chinmays-MacBook-Pro:chapter12 office$ gcc -fPIC -shared -std=gnu99 -o maths_module.o maths_module.c
Chinmays-MacBook-Pro:chapter12 office$ pwd
/Users/office/Desktop/Stuff/Redis-Getting-Started/chapter12
Chinmays-MacBook-Pro:chapter12 office$ redis-cli
127.0.0.1:6379> MODULE LOAD /Users/office/Desktop/Stuff/Redis-Getting-Started/chapter12/maths_module.o
OK
127.0.0.1:6379> maths.multiply 2 3
(integer) 6
127.0.0.1:6379> maths.add 2 3
(integer) 5
127.0.0.1:6379> maths.subtract 2 3
(integer) 1
127.0.0.1:6379> maths.subtract 3 2
(integer) -1
127.0.0.1:6379> 
```


* Reference - https://redis.io/topics/modules-intro
