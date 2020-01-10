<?php

declare(strict_types=1);

namespace App\Domain\Entity;

use InvalidArgumentException;
use UnexpectedValueException;
use BadMethodCallException;
use ReflectionClass;
use JsonSerializable;
use App\Domain\Extensions\MappingTypeCheckWithAnnotation;

trait EntitySupporter
{
    private function getClassProperties()
    {
        return get_class_vars((new ReflectionClass($this))->name);
    }

    private function valueMapping(array $data, string $prefix = '')
    {
        $vars = $this->getClassProperties();

        foreach($vars as $varName => $value)
        {
            $snakeVarName = $prefix . $this->camelToSnake($varName);
            
            if (!array_key_exists($snakeVarName, $data)) {
                throw new InvalidArgumentException("Undefined index in input array, [$snakeVarName]");
            }

            if (!MappingTypeCheckWithAnnotation::check($this, $varName, $data[$snakeVarName])) {
                $value = var_export($data[$snakeVarName], true);
                $className = get_class($this);
                throw new UnexpectedValueException("value [$value] cannot accept in property, [$className::$varName].");
            }

            $this->$varName = $data[$snakeVarName];
        }
    }

    private function snakeToCamel(string $name)
    {
        return preg_replace_callback('/\_([a-zA-Z0-9])/', function($match) {
            return strtoupper($match[1]);
        }, $name);
    }

    private function camelToSnake(string $name)
    {
        $name = strtolower($name[0]) . substr($name, 1);
        return preg_replace_callback('/([A-Z])/', function($match) {
            return '_' . strtolower($match[1]);
        }, $name);
    }

    public function toArray()
    {
        $vars = $this->getClassProperties();

        $arr = [];
        foreach($vars as $varName => $value)
        {
            $value = $this->$varName;
            
            /* if object */
            if(gettype($value) === 'object') {
                if(method_exists($value, 'toArray')) {
                    $value = $value->toArray();
                } elseif($value instanceof JsonSerializable) {
                    $value = $value->JsonSerialize();
                } else {
                    $value = (array)$value;
                }
            }

            $arr[$this->camelToSnake($varName)] = $value;
        }

        return $arr;
    }

    /**
     * for JsonSerializable interface.
     */
    public function JsonSerialize()
    {
        return $this->toArray();
    }

    /* ---------------------------------------------------------- */
    /**
     * Magic methods
     */
    public function __toString()
    {
        if($this instanceof JsonSerializable) {
            return json_encode($this);
        } else {
            return var_export($this->toArray(), true);
        }
    }

    public function __call(string $name, array $args)
    {
        /*
         * setter is startwith `set`
         * getter is startwith `get`
         */ 
        if(preg_match('/^set(.+)$/', $name, $match) && count($args) > 0) {
            return $this->setter($match[1], $args[0]);    
        } elseif(preg_match('^get(.+)$/', $name, $match)) {
            return $this->getter($match[1]);
        }

        throw new UndefinedMethodException("Call unedefined method, [$name]");
    }

    public function __get(string $name)
    {
        return $this->getter($name);
    }

    public function __set(string $name, $value)
    {
        throw new BadMethodCallException('Unsupported property setter.');
    }

    /* ---------------------------------------------------------- */

    /**
     * for default getter
     * @param string $propertyName
     *
     */
    private function getter(string $propertyName) 
    {
        $propertyName = strtolower($propertyName[0]) . substr($propertyName, 1);

        if(!property_exists($this, $propertyName)) {
            throw new InvalidArgumentException("Undefined property [$propertyName]");
        }

        return $this->$propertyName;
    }

    /**
     * for default immutable setter
     * @param string $propertyName
     * @param mixed $value
     * @return self
     */
    private function setter(string $propertyName, $value)
    {
        $propertyName = strtolower($propertyName[0]) . substr($propertyName, 1);

        if(!property_exists($this, $propertyName)) {
            throw new InvalidArgumentException("Undefined property [$propertyName]");
        }

        $clone = clone $this;
        $clone->$propertyName = $value;
        
        return $clone;
    }
}
