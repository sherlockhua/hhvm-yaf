--TEST--
Check for Yaf_Config_Ini
--SKIPIF--
<?php if (!extension_loaded("yaf")) print "skip"; ?>
--FILE--
<?php 
$file = dirname(__FILE__) . "/simple.ini";

$config = new Yaf_Config_Ini($file);
print_r($config);
$config = new Yaf_Config_Ini($file, "extra");
print_r($config);
$config = new Yaf_Config_Ini($file);
$config->longtime = 23424234324;
var_dump($config->readonly());

foreach($config as $key => $value) {
	print_r($key);
}

$sick = @new Yaf_Config_Ini();

var_dump($sick->__isset(1));
var_dump($sick->__get(1));
$sick->total = 1;

var_dump(count($sick));
?>
--EXPECTF--
Yaf_Config_Ini Object
(
    [_cursor:Yaf_Config_Ini:private] => 
    [_config:protected] => Array
        (
            [base] => Array
                (
                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [name] => base
                    [array] => Array
                        (
                            [1] => 1
                            [name] => name
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                )

            [extra] => Array
                (
                    [value] => 2
                    [name] => extra
                    [array] => Array
                        (
                            [name] => new_name
                            [2] => test
                            [1] => 1
                        )

                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                )

            [product] => Array
                (
                    [value] => 2
                    [name] => extra
                    [array] => Array
                        (
                            [name] => new_name
                            [2] => test
                            [1] => 1
                        )

                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                )

            [nocatch] => Array
                (
                    [application] => Array
                        (
                            [dispatcher] => Array
                                (
                                    [throwException] => 
                                    [catchException] => 1
                                )

                            [directory] => APPLICATION_PATH/applcation
                        )

                    [routes] => Array
                        (
                            [rewrite] => Array
                                (
                                    [match] => /yaf/:name/:age
                                    [type] => rewrite
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                        )

                    [value] => 2
                    [name] => extra
                    [array] => Array
                        (
                            [name] => new_name
                            [2] => test
                            [1] => 1
                        )

                    [5] => 5
                )

            [envtest] => Array
                (
                    [env] => 
                    [ini] => 
                    [const] => FOO
                )

        )

    [_readonly:protected] => 1
)
Yaf_Config_Ini Object
(
    [_cursor:Yaf_Config_Ini:private] => 
    [_config:protected] => Array
        (
            [value] => 2
            [name] => extra
            [array] => Array
                (
                    [name] => new_name
                    [2] => test
                    [1] => 1
                )

            [application] => Array
                (
                    [directory] => APPLICATION_PATH/applcation
                )

            [5] => 5
            [routes] => Array
                (
                    [regex] => Array
                        (
                            [type] => regex
                            [match] => ^/ap/(.*)
                            [route] => Array
                                (
                                    [controller] => Index
                                    [action] => action
                                )

                            [map] => Array
                                (
                                    [0] => name
                                    [1] => name
                                    [2] => value
                                )

                        )

                    [simple] => Array
                        (
                            [type] => simple
                            [controller] => c
                            [module] => m
                            [action] => a
                        )

                    [supervar] => Array
                        (
                            [type] => supervar
                            [varname] => c
                        )

                    [rewrite] => Array
                        (
                            [type] => rewrite
                            [match] => /yaf/:name/:value
                            [route] => Array
                                (
                                    [controller] => Index
                                    [action] => action
                                )

                        )

                )

        )

    [_readonly:protected] => 1
)
bool(true)
baseextraproductnocatchenvtestbool(false)
NULL
int(0)
