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

/*
    <<__Native>>
    protected function display();

    <<__Native>>
    public function getRequest();
    
    <<__Native>>
    public function getResponse();

    <<__Native>>
    public function getModuleName();

    <<__Native>>
    public function getView();

    <<__Native>>
    public function initView();

    <<__Native>>
    public function setViewpath();

    <<__Native>>
    public function getViewpath();

    <<__Native>>
    public function forward();
    
    <<__Native>>
    public function redirect();

    <<__Native>>
    public function getInvokeArgs();

    <<__Native>>
    public function getInvokeArg();

    <<__Native>>
    final public function __construct();

    <<__Native>>
    final private function __clone();
*/
}
