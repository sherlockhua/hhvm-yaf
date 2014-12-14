<?hh
class calc {
    public function __construct(){}

     <<__Native>>
     public function add(int $a, int $b):int;
}

abstract class Yaf_Controller {

    public    $actions      = NULL;
    protected $_module      = NULL;
    protected $_name        = NULL;
    protected $_request     = NULL;
    protected $_response    = NULL;
    protected $_invoke_args = NULL;
    protected $_view        = NULL;

    <<__Native>>
    protected function render(string $tpl, ?array $parameters = NULL):mixed;

    <<__Native>>
    public function test():mixed;
}

abstract class Yaf_Action_Abstract extends Yaf_Controller{

    protected  $_controller = NULL;

    public function __construct(){}

     <<__Native>>
    public function getController():mixed;

    abstract public function execute();
}
 
