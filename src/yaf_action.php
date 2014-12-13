<?hh

abstract class Yaf_Action_Abstract extends Yaf_Controller_Abstract
{
    protected  $_controller = null;

    public function __construct(){}

     <<__Native>>
    public function getController():mixed;

    abstract public function execute();
}
