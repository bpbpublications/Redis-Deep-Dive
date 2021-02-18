/*
* Title - Redis Module calculator example
* Developer - Suyog Kale, Chinmay Kulkarni
* Description - Your first helloworld redis module which echo user inputs
* Test - Include module in your redis server and go to redis-cli then enter sdk.module.helloworld $your input$
* Reference - https://redis.io/topics/modules-intro
*/

/*
Include C header file called redismodule.h, its an api defination file
*/
#include "redismodule.h"
#include <stdio.h>
#include <stdlib.h>
/*
To create a new command, It needs the context, the command name, and the function pointer of the function implementing the command, 
which must have the following prototype:
Prototype - int mycommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
*/
int MathMultiply(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
  //Return an error if sufficient inputs are not provided
  if (argc != 3)
  {
    return RedisModule_WrongArity(ctx);
  }

  long long number1;
  long long number2;
  if (RedisModule_StringToLongLong(argv[1], &number1) != REDISMODULE_OK)
    return RedisModule_ReplyWithError(ctx, "Invalid Argument");
  if (RedisModule_StringToLongLong(argv[2], &number2) != REDISMODULE_OK)
    return RedisModule_ReplyWithError(ctx, "Invalid Argument");
  //Ask Redis to automatically manage the resource and memory in our command handler, by simply calling RedisModule_AutoMemory(ctx).
  RedisModule_AutoMemory(ctx);
  long long sum = number1 * number2;
  //Echo the user input
  return RedisModule_ReplyWithLongLong(ctx, sum);
}

int MathAdd(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
  //Return an error if sufficient inputs are not provided
  if (argc != 3)
  {
    return RedisModule_WrongArity(ctx);
  }

  long long number1;
  long long number2;
  if (RedisModule_StringToLongLong(argv[1], &number1) != REDISMODULE_OK)
    return RedisModule_WrongArity(ctx);
  if (RedisModule_StringToLongLong(argv[2], &number2) != REDISMODULE_OK)
    return RedisModule_WrongArity(ctx);
  //Ask Redis to automatically manage the resource and memory in our command handler, by simply calling RedisModule_AutoMemory(ctx).
  RedisModule_AutoMemory(ctx);
  long long sum = number1 + number2;
  //Echo the user input
  return RedisModule_ReplyWithLongLong(ctx, sum);
}

int MathSubtract(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
  //Return an error if sufficient inputs are not provided
  if (argc != 3)
  {
    return RedisModule_WrongArity(ctx);
  }

  long long number1;
  long long number2;
  if (RedisModule_StringToLongLong(argv[1], &number1) != REDISMODULE_OK)
    return RedisModule_WrongArity(ctx);
  if (RedisModule_StringToLongLong(argv[2], &number2) != REDISMODULE_OK)
    return RedisModule_WrongArity(ctx);
  //Ask Redis to automatically manage the resource and memory in our command handler, by simply calling RedisModule_AutoMemory(ctx).
  RedisModule_AutoMemory(ctx);
  long long sum = number2 - number1;
  //Echo the user input
  RedisModule_ReplyWithLongLong(ctx, sum);
  return REDISMODULE_OK;
}

/*
The Init function announces the Redis core that the module has a given name, its version (that is reported by MODULE LIST), and that is willing to use a specific version of the API.
If the API version is wrong, the name is already taken, or there are other similar errors, the function will return REDISMODULE_ERR, and the module OnLoad function should return ASAP with an error.
Before the Init function is called, no other API function can be called, otherwise the module will segfault and the Redis instance will crash

Redis modules API calls all take as first argument the context of the module, so that they have a reference to the module calling it, to the command and client executing a given command, and so forth.
*/
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{

  //Define your module name and version
  if (RedisModule_Init(ctx, "maths.module", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;
  /*
  RedisModule_CreateCommand() - to register commands into the Redis core
  Prototype - int RedisModule_CreateCommand(RedisModuleCtx *ctx, const char *name, RedisModuleCmdFunc cmdfunc, const char *strflags, int firstkey, int lastkey, int keystep);
  Here we define your own command and configure your command function to process
  "readonly" - Here you are not writing or modifiying and data, so configure readonly permissions
  */
  if (RedisModule_CreateCommand(ctx, "maths.multiply", MathMultiply, "readonly", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;
  if (RedisModule_CreateCommand(ctx, "maths.add", MathAdd, "readonly", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;
  if (RedisModule_CreateCommand(ctx, "maths.subtract", MathSubtract, "readonly", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  return REDISMODULE_OK;
}
