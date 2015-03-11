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
    //protected function render(string $tpl, ?array $parameters = NULL):mixed;
    public function render(string $tpl, ?array $parameters = NULL):mixed;

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
     public function setBaseUri(mixed $uri):mixed;

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

    //<<__Native("ActRec")>>
     <<__Native>>
    public function execute():mixed;
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

    static public $_app = NULL;
    protected        $_modules = NULL;
    protected bool   $_running = false;

    //TODO need set default value for _environ
    protected string $_environ = NULL;
    protected int    $_err_no = 0;
    protected string $_err_msg = '';

    public static function app() {
        return self::$_app;
    }

    public static function get_app() {
        return self::$_app;
    }

    public static function set_app($app) {
        self::$_app = $app;
    }

    <<__Native>>
    public function __construct(mixed $config, ?mixed $section = NULL):void;

    <<__Native>>
    public function run():mixed;

    <<__Native("ActRec")>>
    public function execute(string $func, ...):mixed;

    <<__Native>>
    public function environ():mixed;

    <<__Native>>
    public function bootstrap():mixed;

    <<__Native>>
    public function getConfig():mixed;

    <<__Native>>
    public function getModules():mixed;

    <<__Native>>
    public function getDispatcher():mixed;

    <<__Native>>
    public function setAppDirectory(string $name):mixed;

    <<__Native>>
    public function getAppDirectory():mixed;

    <<__Native>>
    public function getLastErrorNo():mixed;

    <<__Native>>
    public function getLastErrorMsg():mixed;

    <<__Native>>
    public function clearLastError():mixed;

    <<__Native>>
    public function __destruct():void;

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
    public function __construct():void;

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
    public function setView(mixed $view):mixed;

    <<__Native>>
    public function setRequest(mixed $request):mixed;

    <<__Native>>
    public function getApplication():mixed;

    <<__Native>>
    public function getRouter():mixed;

    <<__Native>>
    public function getRequest():mixed;

    <<__Native>>
    public function setErrorHandler(string $callback, ?mixed $type=NULL):mixed;

    <<__Native>>
    public function setDefaultModule(mixed $module):mixed;

    <<__Native>>
    public function setDefaultController(mixed $controller):mixed;

    <<__Native>>
    public function setDefaultAction(mixed $action):mixed;

    <<__Native>>
    public function returnResponse(?mixed $res = NULL):mixed;

    <<__Native>>
    public function autoRender(?mixed $res = NULL):mixed;

    <<__Native>>
    public function flushInstantly(?mixed $flush = NULL):mixed;

    static public function getInstance() {
        return self::$_instance;
    }

    static public function setInstance($instance) {
        self::$_instance = $instance;
    }

    <<__Native>>
    public function dispatch(mixed $request):mixed;

    <<__Native>>
    public function throwException(?mixed $flag = NULL):mixed;

    <<__Native>>
    public function catchException(?mixed $flag = NULL):mixed;

    <<__Native>>
    public function registerPlugin(mixed $plugin):mixed;
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
        return true;
    } 

    return false;
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

abstract class Yaf_Bootstrap_Abstract {

} 

final class Yaf_Router {
    protected $_routes = NULL;
    protected $_current = NULL;

    <<__Native>>
    public function __construct():void;

    <<__Native>>
    public function addRoute(string $name, mixed $value):mixed;

    <<__Native>>
    public function addConfig(mixed $value):mixed;

    <<__Native>>
    public function route(mixed $request):mixed;

    <<__Native>>
    public function getRoute(string $name):mixed;

    <<__Native>>
    public function getRoutes():mixed;

    <<__Native>>
    public function getCurrentRoute():mixed;

}

abstract class Yaf_Route_Interface {
    abstract public function route(mixed $request):mixed;
}

final class Yaf_Route_Map extends Yaf_Route_Interface {
    protected $_ctl_router = false;
    protected $_delimeter = NULL;

    <<__Native>>
    public function __construct(?mixed $controller_prefer = NULL, ?mixed $delim = NULL):void;

    <<__Native>>
    public function route(mixed $request):mixed;
}

final class Yaf_Route_Static extends Yaf_Route_Interface {

    <<__Native>>
    public function route(mixed $request):mixed;

    <<__Native>>
    public function match():mixed;
}

abstract class Yaf_Plugin_Abstract {
    <<__Native>>
    public function routerStartup(mixed $request, mixed $response):mixed; 

    <<__Native>>
    public function routerShutdown(mixed $request, mixed $response):mixed;

    <<__Native>>
    public function dispatchLoopStartup(mixed $request, mixed $response):mixed;

    <<__Native>>
    public function dispatchLoopShutdown(mixed $request, mixed $response):mixed;

    <<__Native>>
    public function preDispatch(mixed $request, mixed $response):mixed;

    <<__Native>>
    public function postDispatch(mixed $request, mixed $response):mixed;

    <<__Native>>
    public function preResponse(mixed $request, mixed $response):mixed;
}

class Yaf_Loader{
	protected $_library = null;
	protected $_global_library = null;
	static protected $_instance = null;

    <<__Native>>
    public function __construct():void;

    <<__Native>>
    private function __clone():void;

    <<__Native>>
    private function __sleep():void;

    <<__Native>>
    private function __wakeup():void;

    <<__Native>>
    public function autoload(string $class_name):mixed;

    <<__Native>>
    static public function getInstance(?mixed $library_path = NULL, 
        ?mixed $global_path = NULL):mixed;

    <<__Native>>
    public function registerLocalNamespace(mixed $namespace):mixed;

    <<__Native>>
    public function getLocalNamespace():mixed;

    <<__Native>>
    public function clearLocalNamespace():mixed;

    <<__Native>>
    public function isLocalName(mixed $name):mixed;

    <<__Native>>
    static public function import(string $file):mixed;

    <<__Native>>
    public function setLibraryPath(string $path, ?mixed $is_global = NULL):mixed;

    <<__Native>>
    public function getLibraryPath(?mixed $is_global = NULL):mixed;

    public static function get_instance() {
        return self::$_instance;
    }

    public static function set_instance($instance) {
        self::$_instance = $instance;
    }

}

final class Yaf_Registry {
    protected static $_instance = null;
    protected $_entries = null;
    
    <<__Native>>
    public function __construct():void;

    <<__Native>>
    private function __clone():void;

    <<__Native>>
    static public function get(string $name):mixed;

    <<__Native>>
    static public function set(string $name, mixed $value):mixed;

    <<__Native>>
    static public function has(string $name):mixed;

    <<__Native>>
    static public function del(string $name):mixed;

    static public function  get_instance() {
        return self::$_instance;
    }

    static public function set_instance($instance) {
        self::$_instance = $instance;
    }

}

class Yaf_Exception extends Exception 
{
    protected $message = NULL;
    protected $code = 0; 
    protected $previous = NULL;

    <<__Native>>
    public function __construct(?mixed $message = NULL, 
        ?mixed $code = NULL, ?mixed $previous = NULL):void;

    //<<__Native>>
    //public function getPrevious():mixed;
}

class Yaf_Exception_StartupError extends Yaf_Exception{
}

class Yaf_Exception_RouterFailed extends Yaf_Exception{
}

class Yaf_Exception_DispatchFailed extends Yaf_Exception{
}

class Yaf_Exception_LoadFailed extends Yaf_Exception{
}

class Yaf_Exception_LoadFailed_Module extends Yaf_Exception_LoadFailed{
}

class Yaf_Exception_LoadFailed_Controller extends Yaf_Exception_LoadFailed{
}

class Yaf_Exception_LoadFailed_Action extends Yaf_Exception_LoadFailed{
}

class Yaf_Exception_LoadFailed_View extends Yaf_Exception_LoadFailed{
}

class Yaf_Exception_TypeError extends Yaf_Exception {
}
