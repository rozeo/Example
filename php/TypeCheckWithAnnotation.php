<?php

namespace App\Domain\Extensions;

use DocBlockReader\Reader;

class TypeCheckWithAnnotation
{
    private static $annotationCache = [];

    public static function checkType($classObject, string $propertyName, $value): bool
    {
        $cache = self::getAnnotationCache($classObject, $propertyName);
    
        return self::checkTypeAnnotation($cache, $value);
    }

    public static function isHidden($classObject, string $propertyName): bool
    {
        $cache = self::getAnnotationCache($classObject, $propertyName);

        return !!$cache->getParameter('hidden');
    }

    private static function getAnnotationCache($classObject, string $propertyName): Reader
    {
        $className = $classObject;
        if (gettype($classObject) === 'object') {
            $className = get_class($classObject);
        }

        $cacheName = $className . '::' . $propertyName;

        if (array_key_exists($cacheName, self::$annotationCache)) {
            return self::$annotationCache[$cacheName];
        }

        $annotation =  self::$annotationCache[$cacheName] = new Reader($className, $propertyName, 'property');

        return $annotation;
    }

    private static function checkTypeAnnotation(Reader $reader, $value): bool
    {
        $types = $reader->getParameter('var');
        
        if (empty($types)) {
            return true;
        }

        $valueType = strtolower(gettype($value));
        $valueObjectType = ($valueType === 'object') ? get_class($value): '';

        foreach (preg_split('/\|/', $types) as $typeName)
        {
            if ($valueObjectType !== '' && $valueObjectType === $typeName) {
                return true;
            } elseif ($valueType === $typeName) {
                return true;
            }
        }

        return false;
    }
}


