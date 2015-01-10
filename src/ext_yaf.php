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
    const SCHEME_HTTP = http;
    const SCHEME_HTTPS = https;

    public $module = null;
    public $controller = null;
    public $action = null;
    public $method = null;
    protected $params = null;
    protected $language = null;
    protected $_exception = null;

    protected $_base_uri = '';
    protected $uri = '';
    protected $dispatched = false;
    protected $routed = false;

     <<__Native>>
    public function isGet():bool;

     <<__Native>>
    public function isPost():bool;

     <<__Native>>
    public function isPut():bool;

     <<__Native>>
    public function isHead():bool;

     <<__Native>>
    public function isOptions():bool;

     <<__Native>>
     public function isCli():bool;

     <<__Native>>
     public function isXmlHttpRequest():bool;

     <<__Native>>
     public function getServer(mixed $name, ?mixed $default = NULL):mixed;

     <<__Native>>
    public function getEnv(mixed $name, ?mixed $default=NULL):mixed;

     <<__Native>>
     public function setParam(mixed $name, ?mixed $value = NULL):mixed;

     <<__Native>>
     public function getParam(string $name, ?mixed $default = NULL):mixed;

     <<__Native>>
     public function getParams():mixed;

     <<__Native>>
     public function getException():mixed;

     <<__Native>>
     public function getModuleName():mixed;

     <<__Native>>
     public function getControllerName():mixed;

     <<__Native>>
     public function getActionName():mixed;

     <<__Native>>
     public function setModuleName(mixed $module):mixed;

     <<__Native>>
     public function setControllerName(mixed $name):mixed;

     <<__Native>>
     public function setActionName(mixed $name):mixed;

     <<__Native>>
     public function getMethod():mixed;

     <<__Native>>
     public function getLanguage():mixed;

     <<__Native>>
     public function setBaseUri(mixed $uri):bool;

     <<__Native>>
     public function getBaseUri():mixed;

     <<__Native>>
     public function getRequestUri():mixed;

     <<__Native>>
     public function setRequestUri(mixed $uri):bool;

     <<__Native>>
     public function isDispatched():bool;

     <<__Native>>
     public function setDispatched(bool $dispathed):bool;

     <<__Native>>
     public function isRouted():bool;

     <<__Native>>
     public function setRouted(bool $routed):bool;
}

class yaf_view {
    public function render($path, $params)
    {
        echo "hello, yaf view render\n";
        print_r($path);
        print_r($params);
    }
}

class Yaf_Request_Http extends Yaf_Request_Abstract  {
    const SCHEME_HTTP = http;
    const SCHEME_HTTPS = https;

     <<__Native>>
    public function getQuery(string $name, mixed $def = NULL):mixed;

     <<__Native>>
    public function getRequest(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getPost(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getCookie(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getFiles(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function get(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function isXmlHttpRequest():bool;

    <<__Native>>
    public function __construct(?mixed $request_uri = NULL, ?mixed $base_uri = NULL):void;

    <<__Native>>
    final private function __clone():void;
}

class Yaf_Request_Simple extends Yaf_Request_Abstract  {

     <<__Native>>
    public function getQuery(string $name, mixed $def = NULL):mixed;

     <<__Native>>
    public function getRequest(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getPost(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getCookie(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function getFiles(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function get(string $name, mixed $def = NULL):mixed;

    <<__Native>>
    public function isXmlHttpRequest():bool;

    <<__Native>>
    public function __construct(?mixed $method = NULL, ?mixed $module = NULL,
        ?mixed $controller = NULL, ?mixed $action = NULL, ?mixed $params = NULL):void;

    <<__Native>>
    final private function __clone():void;
}

abstract class Yaf_Action_Abstract extends Yaf_Controller_Abstract{

    protected  $_controller = NULL;

    // <<__Native>>
    //final public function __construct():void;

     <<__Native>>
    public function getController():mixed;

    abstract public function execute();
}
 
abstract class Yaf_Response_Abstract {
    protected $_header = NULL;
    protected $_body = NULL;
    protected bool $_sendheader = false;
    const DEFAULT_BODY = content;

    <<__Native>>
    final private function __clone():void;

    <<__Native>>
    public function __construct():void;

    <<__Native>>
    public function __destruct():void;

    <<__Native>>
    public function __toString():mixed;

    <<__Native>>
    public function setBody():void;

    <<__Native>>
    public function appendBody():void;

    <<__Native>>
    public function prependBody():void;

    <<__Native>>
    public function clearBody():void;

    <<__Native>>
    public function getBody():void;

    <<__Native>>
    public function setHeader():void;

    <<__Native>>
    public function setAllHeaders():void;

    <<__Native>>
    public function getHeader():void;

    <<__Native>>
    public function clearHeaders():void;

    <<__Native>>
    public function setRedirect():void;

    <<__Native>>
    public function response():void;
}


