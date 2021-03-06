/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#include "mvc/model/query/builder/delete.h"
#include "mvc/model/query/builder/where.h"
#include "mvc/model/query/builderinterface.h"
#include "mvc/model/query/exception.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/metadata/memory.h"
#include "mvc/model/query.h"
#include "mvc/model/query/scanner.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "db/rawvalue.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/framework/orm.h"

#include "interned-strings.h"

/**
 * Phalcon\Mvc\Model\Query\Builder\Delete
 *
 *<code>
 *$resultset = Phalcon\Mvc\Model\Query\Builder::createDeleteBuilder()
 *   ->table('Robots')
 *   ->where('name = "Peter"')
 *   ->limit(20)
 *   ->orderBy('Robots.id')
 *   ->execute();
 *</code>
 */
zend_class_entry *phalcon_mvc_model_query_builder_delete_ce;

PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, __construct);
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, table);
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, getTable);
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, _compile);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_mvc_model_query_builder_delete___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_mvc_model_query_builder_delete_table, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

static const zend_function_entry phalcon_mvc_model_query_builder_delete_method_entry[] = {
	PHP_ME(Phalcon_Mvc_Model_Query_Builder_Delete, __construct, arginfo_phalcon_mvc_model_query_builder_delete___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Phalcon_Mvc_Model_Query_Builder_Delete, table, arginfo_phalcon_mvc_model_query_builder_delete_table, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Mvc_Model_Query_Builder_Delete, getTable, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Mvc_Model_Query_Builder_Delete, _compile, NULL, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Phalcon\Mvc\Model\Query\Builder\Delete initializer
 */
PHALCON_INIT_CLASS(Phalcon_Mvc_Model_Query_Builder_Delete){

	PHALCON_REGISTER_CLASS_EX(Phalcon\\Mvc\\Model\\Query\\Builder, Delete, mvc_model_query_builder_delete, phalcon_mvc_model_query_builder_where_ce, phalcon_mvc_model_query_builder_delete_method_entry, 0);

	zend_declare_property_long(phalcon_mvc_model_query_builder_delete_ce, SL("_type"), PHQL_T_DELETE, ZEND_ACC_PROTECTED);
	zend_declare_property_null(phalcon_mvc_model_query_builder_delete_ce, SL("_table"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(phalcon_mvc_model_query_builder_delete_ce, SL("_columns"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(phalcon_mvc_model_query_builder_delete_ce, SL("_values"), ZEND_ACC_PROTECTED);

	zend_class_implements(phalcon_mvc_model_query_builder_delete_ce, 1, phalcon_mvc_model_query_builderinterface_ce);

	return SUCCESS;
}

/**
 * Phalcon\Mvc\Model\Query\Builder\Delete constructor
 *
 * @param array $params
 * @param Phalcon\DI $dependencyInjector
 */
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, __construct){

	zval *params = NULL, *dependency_injector = NULL, conditions = {}, bind_params = {}, bind_types = {}, table = {};

	phalcon_fetch_params(0, 0, 2, &params, &dependency_injector);

	/** 
	 * Update the dependency injector if any
	 */
	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		PHALCON_CALL_METHODW(NULL, getThis(), "setdi", dependency_injector);
	}

	if (params && Z_TYPE_P(params) == IS_ARRAY) {

		/** 
		 * Process conditions
		 */
		if (phalcon_array_isset_fetch_str(&conditions, params, SL("conditions")) || phalcon_array_isset_fetch_long(&conditions, params, 0)) {
			PHALCON_CALL_METHODW(NULL, getThis(), "setconditions", &conditions);
		}

		if (phalcon_array_isset_fetch_str(&bind_params, params, SL("bind"))) {
				PHALCON_CALL_METHODW(NULL, getThis(), "setbindparams", &bind_params);
		}

		if (phalcon_array_isset_fetch_str(&bind_types, params, SL("bindTypes"))) {
			PHALCON_CALL_METHODW(NULL, getThis(), "setbindtypes", &bind_types);
		}

		if (phalcon_array_isset_fetch_str(&table, params, SL("table"))) {
			PHALCON_CALL_METHODW(NULL, getThis(), "settable", &table);
		}
	}
}

/**
 * Sets the table to delete from
 *
 * @param string table
 * @return Phalcon\Mvc\Model\Query\Builder\Delete
 */
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, table){

	zval *table;

	phalcon_fetch_params(0, 1, 0, &table);

	if (Z_TYPE_P(table) != IS_STRING) {
		PHALCON_ENSURE_IS_STRING(table);
	}

	phalcon_update_property_zval(getThis(), SL("_table"), table);
	RETURN_THISW();
}

/**
 * Gets the table to delete from
 *
 * @return bool
 */
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, getTable){


	RETURN_MEMBER(getThis(), "_table");
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query_Builder_Delete, _compile){

	zval table = {}, conditions = {}, phql = {}, bind_params = {}, bind_types = {};

	PHALCON_CALL_SELFW(&table, "gettable");

	PHALCON_CALL_SELFW(&conditions, "getconditions");

	PHALCON_CONCAT_SVS(&phql, "DELETE FROM [", &table, "]");

	/** 
	 * Only append conditions if it's string
	 */
	if (Z_TYPE(conditions) == IS_STRING && PHALCON_IS_NOT_EMPTY(&conditions)) {
		PHALCON_SCONCAT_SV(&phql, " WHERE ", &conditions);
	}

	PHALCON_CALL_SELFW(&bind_params, "getbindparams");
	phalcon_update_property_zval(getThis(), SL("_mergeBindParams"), &bind_params);

	PHALCON_CALL_SELFW(&bind_types, "getbindtypes");
	phalcon_update_property_zval(getThis(), SL("_mergeBindTypes"), &bind_types);

	phalcon_update_property_zval(getThis(), SL("_phql"), &phql);
}
