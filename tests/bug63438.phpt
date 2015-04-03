--TEST--
Bug #63438 (Strange behavior with nested rendering)
--SKIPIF--
<?php if (!extension_loaded("yaf")) print "skip"; ?>
--INI--
yaf.use_spl_autoload=1
yaf.lowcase_path=0
--FILE--
<?php

function shut_down() {
   echo "done";
}

register_shutdown_function("shut_down");

function view($file){
    static $view;
   
    $view = new Yaf_View_Simple(dirname(__FILE__));
    return $view->render($file);
}

file_put_contents(dirname(__FILE__) . '/outer.phtml', "1 <?php print view('inner.phtml');?> 3\n");
file_put_contents(dirname(__FILE__) . '/inner.phtml', "2");
print (view('outer.phtml'));

file_put_contents(dirname(__FILE__) . '/outer1.phtml', "1 <?php \$this->display('inner.phtml');?> 3\n");
print (view('outer1.phtml'));

file_put_contents(dirname(__FILE__) . '/outer2.phtml', "1 <?php echo \$this->evaler('2');?> 3\n");
print (view('outer2.phtml'));

file_put_contents(dirname(__FILE__) . '/outer3.phtml', "1 <?php \$this->display('inner3.phtml');?> 3\n");
file_put_contents(dirname(__FILE__) . '/inner3.phtml', "<?php undefined_function(); ?>");
print (view('outer3.phtml'));
?>
--CLEAN--
<?php
@unlink(dirname(__FILE__) . '/outer.phtml');
@unlink(dirname(__FILE__) . '/inner.phtml');
?>
--EXPECTF--
1 2 3
1 2 3
1 2 3
%shphp%s
Fatal error: Call to undefined function undefined_function() in %sinner3.phtml on line %d
1 done
