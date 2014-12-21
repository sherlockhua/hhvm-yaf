<?hh

abstract class Yaf_Controller_Abstract {

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

    <<__Native>>
    protected function display(string $tpl, ?array $parameters = NULL):mixed;

    <<__Native>>
    public function getRequest():mixed;
    
    <<__Native>>
    public function getResponse():mixed;

    <<__Native>>
    public function getModuleName():mixed;

    <<__Native>>
    public function getView():mixed;

    <<__Native>>
    public function initView(?array $options = NULL):mixed;

    <<__Native>>
    public function setViewpath(string $view_directory):bool;

    <<__Native>>
    public function getViewpath():mixed;

    <<__Native>>
    public function forward(mixed $module, ?mixed $controller = NULL, 
        ?mixed $action = NULL, ?mixed $paramters = NULL):mixed;
    
    <<__Native>>
    public function redirect(string $url):mixed;

    <<__Native>>
    public function getInvokeArgs():mixed;

    <<__Native>>
    public function getInvokeArg(string $name):mixed;

    <<__Native>>
    final public function __construct(object $request, object $response, 
        object $view, ?array $invokeArgs = NULL):void;

    <<__Native>>
    final private function __clone():void;
}

abstract class Yaf_Request_Abstract  {
    public $module = null;
    public $controller = null;
    public $action = null;
    public $method = null;
    protected $params = null;
    protected $language = null;
    protected $_exception = null;

    protected $_base_uri = '';
    protected $_uri = '';
    protected $dispatched = false;
    protected $routed = false;

     <<__Native>>
    public function isGet():bool;

/*
     <<__Native>>
    public function isPost():mixed;

     <<__Native>>
    public function isPut();

     <<__Native>>
    public function isHead();

     <<__Native>>
    public function isOptions();

     <<__Native>>
     public function isCli();

     <<__Native>>
     public function isXmlHttpRequest());

     <<__Native>>
     public function getServer();

     <<__Native>>
    public function getEnv();

     <<__Native>>
     public function setParam();

     <<__Native>>
     public function getParam();

     <<__Native>>
     public function getParams();

     <<__Native>>
     public function getException();

     <<__Native>>
     public function getModuleName();

     <<__Native>>
     public function getControllerName();

     <<__Native>>
     public function getActionName();

     <<__Native>>
     public function setModuleName();

     <<__Native>>
     public function setControllerName();

     <<__Native>>
     public function setActionName();

     <<__Native>>
     public function getMethod();

     <<__Native>>
     public function getLanguage();

     <<__Native>>
     public function setBaseUri();

     <<__Native>>
     public function getBaseUri();

     <<__Native>>
     public function getRequestUri();

     <<__Native>>
     public function setRequestUri();

     <<__Native>>
     public function isDispatched();

     <<__Native>>
     public function setDispatched();

     <<__Native>>
     public function isRouted();

     <<__Native>>
     public function setRouted();
 */
}

class yaf_view {
    public function render($path, $params)
    {
        echo "hello, yaf view render\n";
        print_r($path);
        print_r($params);
    }
}

abstract class Yaf_Action_Abstract extends Yaf_Controller_Abstract{

    protected  $_controller = NULL;

    // <<__Native>>
    //final public function __construct():void;

     <<__Native>>
    public function getController():mixed;

    abstract public function execute();
}
 
