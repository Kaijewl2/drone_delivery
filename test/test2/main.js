// Source - https://stackoverflow.com/a/67080654
// Posted by kalwalt
// Retrieved 2026-08-16, License - CC BY-SA 4.0

// main.js

Module.onRuntimeInitialized = async function(){
    // uncommentthe code below to output the Module in the console
    // console.log('Module loaded: ', Module);
    var instance = new Module.Hello(); // this will print "Hello world!!!""
    console.log(instance); // this will pint the class as an object "Hello{}""
    instance.saySomething(); // this will print "something"
}

