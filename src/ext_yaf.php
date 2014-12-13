<?hh
class calc {
    public function __construct(){}

     <<__Native>>
     public function add(int $a, int $b):int;
}

class Yaf_Controller {

    <<__Native>>
    public function test():mixed;
}

abstract class Yaf_Action_Abstract extends Yaf_Controller{

    protected  $_controller = null;

    public function __construct(){}

     <<__Native>>
    public function getController():mixed;

    abstract public function execute();
}
 
