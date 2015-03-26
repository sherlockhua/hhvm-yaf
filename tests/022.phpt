--TEST--
Check for Yaf_Application
--SKIPIF--
<?php if (!extension_loaded("yaf")) print "skip"; ?>
--INI--
--FILE--
<?php 
$config = array(
	"application" => array(
		"directory" => realpath(dirname(__FILE__)),
		"dispatcher" => array(
			"catchException" => 0,
			"throwException" => 0,
		),
	),
);

$app = new Yaf_Application($config);
var_dump($app->getAppDirectory());
$dir = $app->getAppDirectory() . "/tmp";
mkdir($dir);
$app->setAppDirectory($dir);
var_dump($app->getAppDirectory());
$app->run();
?>
--CLEAN--
<?php
rmdir(dirname(__FILE__) . "/tmp");
?>
--EXPECTF--
string(%d) "%stests"
%shphp%s
string(%d) "%stmp"
%shphp%s
Fatal error: Failed opening controller script %stmp%ccontrollers%cIndex.php: No such file or directory in %s022.php on line %d
