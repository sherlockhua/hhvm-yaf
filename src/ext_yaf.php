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
    public function __toString():string;

    <<__Native>>
    public function setBody(string $body, ?mixed $name = NULL):mixed;

    <<__Native>>
    public function appendBody(string $body, ?mixed $name=NULL):mixed;

    <<__Native>>
    public function prependBody(string $body, ?mixed $name = NULL):mixed;

    <<__Native>>
    public function clearBody(?mixed $name=NULL):mixed;

    <<__Native>>
    public function getBody(?mixed $name = NULL):mixed;

    <<__Native>>
    public function setHeader():mixed;

    <<__Native>>
    public function setAllHeaders():mixed;

    <<__Native>>
    public function getHeader():mixed;

    <<__Native>>
    public function clearHeaders():mixed;

    <<__Native>>
    public function setRedirect(string $url):bool;

    <<__Native>>
    public function response():mixed;
}

class Yaf_Response_Http extends Yaf_Response_Abstract {
    protected bool $_sendheader = true;
    protected int  $_response_code = 200;

    <<__Native>>
    public function __construct():void;
}

class Yaf_Response_Cli extends Yaf_Response_Abstract {

    <<__Native>>
    public function __construct():void;
}

final class Yaf_Application {

    protected        $config = NULL;
    protected        $dispatcher = NULL;
    static protected $_app = NULL;
    protected        $_modules = NULL;
    protected bool   $_running = false;

    //TODO need set default value for _environ
    protected string $_environ = NULL;
    protected int    $_err_no = 0;
    protected string $_err_msg = '';

    <<__Native>>
    public function __construct(mixed $config, ?mixed $section = NULL):void;

    <<__Native>>
    public function run():mixed;

    <<__Native>>
    public function execute():mixed;

    //TODO yaf_application_environ::environ method not finished
    <<__Native>>
    public function bootstrap():mixed;

    <<__Native>>
    public function getConfig():mixed;

    <<__Native>>
    public function getModules():mixed;

    <<__Native>>
    public function getDispatcher():mixed;

    <<__Native>>
    public function setAppDirectory():mixed;

    <<__Native>>
    public function getAppDirectory():mixed;

    <<__Native>>
    public function getLastErrorNo():mixed;

    <<__Native>>
    public function getLastErrorMsg():mixed;

    <<__Native>>
    public function clearLastError():mixed;

    <<__Native>>
    public function __destruct():mixed;

    <<__Native>>
    private function __clone():void;

    <<__Native>>
    private function __sleep():void;
    
    <<__Native>>
    private function __wakeup():void;
}

final class Yaf_Dispatcher {
    protected $_router = NULL;
    protected $_view = NULL;
    protected $_request = NULL;
    protected $_plugins = NULL;
    static protected $_instance = NULL; 

    protected bool $_auto_render = true;
    protected bool $_return_response = false;
    protected bool $_instantly_flush = false;

    protected $_default_module = NULL;
    protected $_default_controller = NULL;
    protected $_default_action = NULL;

    <<__Native>>
    private function __construct():void;

    <<__Native>>
    private function __clone():void;

    <<__Native>>
    private function __sleep():void;

    <<__Native>>
    private function __wakeup():void;

    <<__Native>>
    public function enableView():mixed;

    <<__Native>>
    public function disableView():mixed;

    <<__Native>>
    public function initView(mixed $tpl_dir, ?mixed $options):mixed;

    <<__Native>>
    public function setView():void;

    <<__Native>>
    public function setRequest():void;

    <<__Native>>
    public function getApplication():void;

    <<__Native>>
    public function getRouter():void;

    <<__Native>>
    public function getRequest():void;

    <<__Native>>
    public function setErrorHandler():void;

    <<__Native>>
    public function setDefaultModule():void;

    <<__Native>>
    public function setDefaultController():void;

    <<__Native>>
    public function setDefaultAction():void;

    <<__Native>>
    public function returnResponse():void;

    <<__Native>>
    public function autoRender():void;

    <<__Native>>
    public function flushInstantly():void;

    <<__Native>>
    static public function getInstance():void;

    <<__Native>>
    public function dispatch():void;

    <<__Native>>
    public function throwException():void;

    <<__Native>>
    public function catchException():void;

    <<__Native>>
    public function registerPlugin():void;
}

abstract class Yaf_View_Interface {
    <<__Native>>
    public function assign(mixed $name, ?mixed $value=NULL):mixed;

    <<__Native>>
    public function display(mixed $tpl, ?mixed $vars = NULL):mixed;

    <<__Native>>
    public function render(mixed $tpl, ?mixed $vars = NULL):mixed;

    <<__Native>>
    public function setScriptPath():void;

    <<__Native>>
    public function getScriptPath():void;
}

class Yaf_View_Simple extends Yaf_View_Interface {
    protected $_tpl_vars = NULL;
    protected $_tpl_dir = NULL;
    protected $_options = NULL;

    <<__Native>>
    public function __construct(mixed $tpl_dir, ?mixed $options = NULL):void;

    <<__Native>>
    public function __isset(string $name):bool;

    <<__Native>>
    public function get(?mixed $name = NULL):mixed;

    <<__Native>>
    public function assign(mixed $name, ?mixed $value=NULL):mixed;

    <<__Native>>
    public function render(mixed $tpl, ?mixed $vars = NULL):mixed;

    <<__Native>>
    public function test():mixed;

    <<__Native>>
    public function evaler(mixed $tpl, ?mixed $vars = NULL):mixed;

    <<__Native>>
    public function display(mixed $tpl, ?mixed $vars = NULL):mixed;

    <<__Native>>
    public function assignRef(string $name, mixed& $value):mixed;

    <<__Native>>
    public function clear(?mixed $name = NULL):mixed;

    <<__Native>>
    public function setScriptPath(mixed $tpl_dir):mixed;

    <<__Native>>
    public function getScriptPath():mixed;

    <<__Native>>
    public function __get(?mixed $name = NULL):mixed;

    <<__Native>>
    public function __set(mixed $name, ?mixed $value=NULL):mixed;
}

function yaf_loader_import(string $path)
{
    if ( file_exists($path) ) { 
        extract($GLOBALS);
        require_once $path;
    } 
}

abstract class Yaf_Config_Abstract {
    protected $_config = NULL;
    protected bool $_readonly = true;

    <<__Native>>
    public function __construct():void;

    abstract public function get(?mixed $name):mixed;
    abstract public function set(string $name, mixed $value):mixed;
    abstract public function readonly():mixed;
    abstract public function toArray():mixed;
}

class Yaf_Config_Simple extends Yaf_Config_Abstract {
    protected bool $_readonly = false;
    private $_cursor = NULL;

    <<__Native>>
    public function __construct(mixed $config, ?mixed $readonly = NULL):void;

    <<__Native>>
    public function __isset(string $name):bool;

    <<__Native>>
    public function get(?mixed $name):mixed;

    <<__Native>>
    public function set(string $name, mixed $value):mixed;

    <<__Native>>
    public function count():mixed;

    <<__Native>>
    public function offsetUnset(string $name):mixed;

    <<__Native>>
    public function rewind():mixed;

    <<__Native>>
    public function current():mixed;

    <<__Native>>
    public function next():mixed;

    <<__Native>>
    public function valid():mixed;

    <<__Native>>
    public function key():mixed;

    <<__Native>>
    public function readonly():mixed;

    <<__Native>>
    public function toArray():mixed;

    <<__Native>>
    public function __set(string $name, mixed $value):mixed;

    <<__Native>>
    public function __get(?mixed $name = NULL):mixed;

    <<__Native>>
    public function offsetGet(?mixed $name = NULL):mixed;

    <<__Native>>
    public function offsetExists(string $name):bool;

    <<__Native>>
    public function offsetSet(string $name, mixed $value):mixed;
}

class Yaf_Config_Ini extends Yaf_Config_Abstract 
{
    private $_cursor = NULL;
    <<__Native>>
    public function __construct(mixed $filename, ?mixed $section = NULL):void;

    <<__Native>>
    public function __isset(string $name):bool;

    <<__Native>>
    public function get(?mixed $name):mixed;

    <<__Native>>
    public function set(string $name, mixed $value):mixed;

    <<__Native>>
    public function count():mixed;

    <<__Native>>
    public function rewind():mixed;

    <<__Native>>
    public function current():mixed;

    <<__Native>>
    public function next():mixed;

    <<__Native>>
    public function valid():mixed;

    <<__Native>>
    public function key():mixed;

    <<__Native>>
    public function readonly():mixed;

    <<__Native>>
    public function toArray():mixed;

    <<__Native>>
    public function offsetUnset(string $name):mixed;

    <<__Native>>
    public function __set(string $name, mixed $value):mixed;

    <<__Native>>
    public function __get(?mixed $name = NULL):mixed;

    <<__Native>>
    public function offsetGet(?mixed $name = NULL):mixed;

    <<__Native>>
    public function offsetExists(string $name):bool;

    <<__Native>>
    public function offsetSet(string $name, mixed $value):mixed;

}

