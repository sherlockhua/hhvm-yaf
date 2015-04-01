--TEST--
Check for Yaf_Config_Ini with env
--SKIPIF--
<?php if (!extension_loaded("yaf")) print "skip"; ?>
--INI--
yaf.directory=/foo/bar
--FILE--
<?php 
//TODO hhvm 3.1 not effective
//putenv("FOO=bar");
define("FOO", "Dummy");
print_r(new Yaf_Config_Ini(
	dirname(__FILE__) . "/simple.ini", "envtest"));
?>
--EXPECTF--
Yaf_Config_Ini Object
(
    [_cursor:Yaf_Config_Ini:private] => 
    [_config:protected] => Array
        (
            [env] => 
            [ini] => 
            [const] => Dummy
        )

    [_readonly:protected] => 1
)
