
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
  |          Marcio Paiva <mpaivabarbosa@gmail.com>                        |
  +------------------------------------------------------------------------+
*/

#include "db/dialect/oracle.h"
#include "db/dialect.h"
#include "db/dialectinterface.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"

/**
 * Phalcon\Db\Dialect\Oracle
 *
 * Generates database specific SQL for the Oracle RBDMS
 */
zend_class_entry *phalcon_db_dialect_oracle_ce;

PHP_METHOD(Phalcon_Db_Dialect_Oracle, getColumnDefinition);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addColumn);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, modifyColumn);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropColumn);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addIndex);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropIndex);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addPrimaryKey);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropPrimaryKey);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addForeignKey);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropForeignKey);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, _getTableOptions);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, createTable);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropTable);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, createView);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropView);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, tableExists);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, viewExists);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeColumns);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, listTables);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, listViews);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeIndexes);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeReferences);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, tableOptions);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, getSqlTable);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, limit);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, select);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, supportsSavepoints);
PHP_METHOD(Phalcon_Db_Dialect_Oracle, supportsReleaseSavepoints);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_db_dialect_oracle_getsqltable, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

static const zend_function_entry phalcon_db_dialect_oracle_method_entry[] = {
	PHP_ME(Phalcon_Db_Dialect_Oracle, getColumnDefinition, arginfo_phalcon_db_dialectinterface_getcolumndefinition, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, addColumn, arginfo_phalcon_db_dialectinterface_addcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, modifyColumn, arginfo_phalcon_db_dialectinterface_modifycolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropColumn, arginfo_phalcon_db_dialectinterface_dropcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, addIndex, arginfo_phalcon_db_dialectinterface_addindex, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropIndex, arginfo_phalcon_db_dialectinterface_dropindex, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, addPrimaryKey, arginfo_phalcon_db_dialectinterface_addprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropPrimaryKey, arginfo_phalcon_db_dialectinterface_dropprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, addForeignKey, arginfo_phalcon_db_dialectinterface_addforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropForeignKey, arginfo_phalcon_db_dialectinterface_dropforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, _getTableOptions, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Phalcon_Db_Dialect_Oracle, createTable, arginfo_phalcon_db_dialectinterface_createtable, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropTable, arginfo_phalcon_db_dialectinterface_droptable, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, createView, arginfo_phalcon_db_dialectinterface_createview, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, dropView, arginfo_phalcon_db_dialectinterface_dropview, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, tableExists, arginfo_phalcon_db_dialectinterface_tableexists, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, viewExists, arginfo_phalcon_db_dialectinterface_viewexists, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, describeColumns, arginfo_phalcon_db_dialectinterface_describecolumns, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, listTables, arginfo_phalcon_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, listViews, arginfo_phalcon_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, describeIndexes, arginfo_phalcon_db_dialectinterface_describeindexes, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, describeReferences, arginfo_phalcon_db_dialectinterface_describereferences, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, tableOptions, arginfo_phalcon_db_dialectinterface_tableoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, getSqlTable, arginfo_phalcon_db_dialect_oracle_getsqltable, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, limit, arginfo_phalcon_db_dialectinterface_limit, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, select, arginfo_phalcon_db_dialectinterface_select, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, supportsSavepoints, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Db_Dialect_Oracle, supportsReleaseSavepoints, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Phalcon\Db\Dialect\Oracle initializer
 */
PHALCON_INIT_CLASS(Phalcon_Db_Dialect_Oracle){

	PHALCON_REGISTER_CLASS_EX(Phalcon\\Db\\Dialect, Oracle, db_dialect_oracle, phalcon_db_dialect_ce, phalcon_db_dialect_oracle_method_entry, 0);

	zend_declare_property_string(phalcon_db_dialect_oracle_ce, SL("_escapeChar"), "", ZEND_ACC_PROTECTED);

	zend_class_implements(phalcon_db_dialect_oracle_ce, 1, phalcon_db_dialectinterface_ce);

	return SUCCESS;
}

/**
 * Gets the column name in Oracle
 *
 * @param Phalcon\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, getColumnDefinition){

	zval *column, size = {}, column_type = {}, column_sql = {}, scale = {};

	phalcon_fetch_params(0, 1, 0, &column);

	if (Z_TYPE_P(column) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Column definition must be an object compatible with Phalcon\\Db\\ColumnInterface");
		return;
	}

	PHALCON_CALL_METHODW(&size, column, "getsize");
	PHALCON_CALL_METHODW(&column_type, column, "gettype");

	switch (phalcon_get_intval(&column_type)) {

		case 0:
			ZVAL_STRING(&column_sql, "INTEGER");
			break;

		case 1:
			ZVAL_STRING(&column_sql, "DATE");
			break;

		case 2:
			PHALCON_CONCAT_SVS(&column_sql, "VARCHAR2(", &size, ")");
			break;

		case 3:
			PHALCON_CALL_METHODW(&scale, column, "getscale");
			PHALCON_CONCAT_SVSVS(&column_sql, "NUMBER(", &size, ",", &scale, ")");
			break;

		case 4:
			ZVAL_STRING(&column_sql, "TIMESTAMP");
			break;

		case 5:
			PHALCON_CONCAT_SVS(&column_sql, "CHAR(", &size, ")");
			break;

		case 6:
			ZVAL_STRING(&column_sql, "TEXT");
			break;

		case 7:
			PHALCON_CALL_METHODW(&scale, column, "getscale");
			PHALCON_CONCAT_SVSVS(&column_sql, "FLOAT(", &size, ",", &scale, ")");
			break;

		case 8:
			ZVAL_STRING(&column_sql, "TINYINT(1)");
			break;

		default:
			PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Unrecognized Oracle data type");
			return;

	}

	RETURN_CTORW(&column_sql);
}

/**
 * Generates SQL to add a column to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addColumn){

	zval *table_name, *schema_name, *column;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &column);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to modify a column in a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, modifyColumn){

	zval *table_name, *schema_name, *column;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &column);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to delete a column from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $columnName
 * @return 	string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropColumn){

	zval *table_name, *schema_name, *column_name;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &column_name);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Index $index
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addIndex){

	zval *table_name, *schema_name, *index;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &index);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to delete an index from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $indexName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropIndex){

	zval *table_name, *schema_name, *index_name;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &index_name);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to add the primary key to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Index $index
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addPrimaryKey){

	zval *table_name, *schema_name, *index;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &index);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to delete primary key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropPrimaryKey){

	zval *table_name, *schema_name;

	phalcon_fetch_params(0, 2, 0, &table_name, &schema_name);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\ReferenceInterface $reference
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, addForeignKey){

	zval *table_name, *schema_name, *reference;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &reference);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to delete a foreign key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $referenceName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropForeignKey){

	zval *table_name, *schema_name, *reference_name;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &reference_name);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to add the table creation options
 *
 * @param array $definition
 * @return array
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, _getTableOptions){

	zval *definition, empty_array = {};

	phalcon_fetch_params(0, 1, 0, &definition);

	array_init(&empty_array);
	RETURN_CTORW(&empty_array);
}

/**
 * Generates SQL to create a table in PostgreSQL
 *
 * @param 	string $tableName
 * @param string $schemaName
 * @param array $definition
 * @return 	string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, createTable){

	zval *table_name, *schema_name, *definition;

	phalcon_fetch_params(0, 3, 0, &table_name, &schema_name, &definition);

	PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Not implemented yet");
}

/**
 * Generates SQL to drop a table
 *
 * @param  string $tableName
 * @param  string $schemaName
 * @param  boolean $ifExists
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropTable){

	zval *table_name, *schema_name, *if_exists = NULL, table = {};

	phalcon_fetch_params(0, 2, 1, &table_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &PHALCON_GLOBAL(z_true);
	}

	if (zend_is_true(schema_name)) {
		PHALCON_CONCAT_VSV(&table, schema_name, ".", table_name);
	} else {
		PHALCON_CPY_WRT(&table, table_name);
	}
	if (zend_is_true(if_exists)) {
		PHALCON_CONCAT_SV(return_value, "DROP TABLE IF EXISTS ", &table);
	} else {
		PHALCON_CONCAT_SV(return_value, "DROP TABLE ", &table);
	}
}

/**
 * Generates SQL to create a view
 *
 * @param string $viewName
 * @param array $definition
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, createView){

	zval *view_name, *definition, *schema_name, view_sql = {}, view = {};

	phalcon_fetch_params(0, 3, 0, &view_name, &definition, &schema_name);

	if (!phalcon_array_isset_fetch_str(&view_sql, definition, SL("sql"))) {
		PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "The index 'sql' is required in the definition array");
		return;
	}

	if (zend_is_true(schema_name)) {
		PHALCON_CONCAT_VSV(&view, view_name, ".", schema_name);
	} else {
		PHALCON_CPY_WRT_CTOR(&view, view_name);
	}

	PHALCON_CONCAT_SVSV(return_value, "CREATE VIEW ", &view, " AS ", &view_sql);
}

/**
 * Generates SQL to drop a view
 *
 * @param string $viewName
 * @param string $schemaName
 * @param boolean $ifExists
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, dropView){

	zval *view_name, *schema_name, *if_exists = NULL, view = {};

	phalcon_fetch_params(0, 2, 1, &view_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &PHALCON_GLOBAL(z_true);
	}

	if (zend_is_true(schema_name)) {
		PHALCON_CONCAT_SVSVS(&view, "`", schema_name, "`.`", view_name, "`");
	} else {
		PHALCON_CONCAT_SVS(&view, "`", view_name, "`");
	}
	if (zend_is_true(if_exists)) {
		PHALCON_CONCAT_SV(return_value, "DROP VIEW IF EXISTS ", &view);
	} else {
		PHALCON_CONCAT_SV(return_value, "DROP VIEW ", &view);
	}
}

/**
 * Generates SQL checking for the existence of a schema.table
 *
 *<code>
 *	var_dump($dialect->tableExists("posts", "blog"));
 *	var_dump($dialect->tableExists("posts"));
 *</code>
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, tableExists){

	zval *table_name, *schema_name = NULL;

	phalcon_fetch_params(0, 1, 1, &table_name, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		PHALCON_CONCAT_SVSVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END RET FROM ALL_TABLES WHERE TABLE_NAME='", table_name, "' AND OWNER = '", schema_name, "'");
	} else {
		PHALCON_CONCAT_SVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END RET FROM ALL_TABLES WHERE TABLE_NAME='", table_name, "'");
	}
}

/**
 * Generates SQL checking for the existence of a schema.view
 *
 * @param string $viewName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, viewExists){

	zval *view_name, *schema_name = NULL;

	phalcon_fetch_params(0, 1, 1, &view_name, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		PHALCON_CONCAT_SVSVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END RET FROM ALL_VIEWS WHERE VIEW_NAME='", view_name, "' AND OWNER='", schema_name, "'");
	} else {
		PHALCON_CONCAT_SVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END RET FROM ALL_VIEWS WHERE VIEW_NAME='", view_name, "'");
	}
}

/**
 * Generates a SQL describing a table
 *
 *<code>
 *	print_r($dialect->describeColumns("posts")); ?>
 *</code>
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeColumns){

	zval *table, *schema = NULL;

	phalcon_fetch_params(0, 1, 1, &table, &schema);

	if (schema && zend_is_true(schema)) {
		PHALCON_CONCAT_SVSVS(return_value, "SELECT TC.COLUMN_NAME, TC.DATA_TYPE, TC.DATA_LENGTH, TC.DATA_PRECISION, TC.DATA_SCALE, TC.NULLABLE, C.CONSTRAINT_TYPE, TC.DATA_DEFAULT, CC.POSITION FROM ALL_TAB_COLUMNS TC LEFT JOIN (ALL_CONS_COLUMNS CC JOIN ALL_CONSTRAINTS C ON (CC.CONSTRAINT_NAME = C.CONSTRAINT_NAME AND CC.TABLE_NAME = C.TABLE_NAME AND CC.OWNER = C.OWNER AND C.CONSTRAINT_TYPE = 'P')) ON TC.TABLE_NAME = CC.TABLE_NAME AND TC.COLUMN_NAME = CC.COLUMN_NAME WHERE TC.TABLE_NAME = '", table, "' AND TC.OWNER = '", schema, "' ORDER BY TC.COLUMN_ID");
	} else {
		PHALCON_CONCAT_SVS(return_value, "SELECT TC.COLUMN_NAME, TC.DATA_TYPE, TC.DATA_LENGTH, TC.DATA_PRECISION, TC.DATA_SCALE, TC.NULLABLE, C.CONSTRAINT_TYPE, TC.DATA_DEFAULT, CC.POSITION FROM ALL_TAB_COLUMNS TC LEFT JOIN (ALL_CONS_COLUMNS CC JOIN ALL_CONSTRAINTS C ON (CC.CONSTRAINT_NAME = C.CONSTRAINT_NAME AND CC.TABLE_NAME = C.TABLE_NAME AND CC.OWNER = C.OWNER AND C.CONSTRAINT_TYPE = 'P')) ON TC.TABLE_NAME = CC.TABLE_NAME AND TC.COLUMN_NAME = CC.COLUMN_NAME WHERE TC.TABLE_NAME = '", table, "' ORDER BY TC.COLUMN_ID");
	}
}

/**
 * List all tables on database
 *
 *<code>
 *	print_r($dialect->listTables("blog")) ?>
 *</code>
 *
 * @param       string $schemaName
 * @return      array
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, listTables){

	zval *schema_name = NULL;

	phalcon_fetch_params(0, 0, 1, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		PHALCON_CONCAT_SVS(return_value, "SELECT TABLE_NAME, OWNER FROM ALL_TABLES WHERE OWNER='", schema_name, "' ORDER BY OWNER, TABLE_NAME");
	} else {
		RETURN_STRING("SELECT TABLE_NAME, OWNER FROM ALL_TABLES ORDER BY OWNER, TABLE_NAME ");
	}
}

/**
 * Generates the SQL to list all views of a schema or user
 *
 * @param string $schemaName
 * @return array
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, listViews){

	zval *schema_name = NULL;

	phalcon_fetch_params(0, 0, 1, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		PHALCON_CONCAT_SVS(return_value, "SELECT VIEW_NAME FROM ALL_VIEWS WHERE OWNER='", schema_name, "' ORDER BY VIEW_NAME");
	} else {
		RETURN_STRING("SELECT VIEW_NAME FROM ALL_VIEWS VIEW_NAME");
	}
}

/**
 * Generates SQL to query indexes on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeIndexes){

	zval *table, *schema = NULL;

	phalcon_fetch_params(0, 1, 1, &table, &schema);

	if (schema && zend_is_true(schema)) {
		PHALCON_CONCAT_SVSVS(return_value, "SELECT I.TABLE_NAME, 0 AS C0, I.INDEX_NAME, IC.COLUMN_POSITION, IC.COLUMN_NAME FROM ALL_INDEXES I JOIN ALL_IND_COLUMNS IC ON I.INDEX_NAME = IC.INDEX_NAME WHERE  I.TABLE_NAME = '", table, "' AND IC.INDEX_OWNER = '", schema, "'");
	} else {
		PHALCON_CONCAT_SVS(return_value, "SELECT I.TABLE_NAME, 0 AS C0, I.INDEX_NAME, IC.COLUMN_POSITION, IC.COLUMN_NAME FROM ALL_INDEXES I JOIN ALL_IND_COLUMNS IC ON I.INDEX_NAME = IC.INDEX_NAME WHERE  I.TABLE_NAME = '", table, "'");
	}
}

/**
 * Generates SQL to query foreign keys on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, describeReferences){

	zval *table, *schema = NULL, sql = {};

	phalcon_fetch_params(0, 1, 1, &table, &schema);

	ZVAL_STRING(&sql, "SELECT AC.TABLE_NAME, CC.COLUMN_NAME, AC.CONSTRAINT_NAME, AC.R_OWNER, RCC.TABLE_NAME R_TABLE_NAME, RCC.COLUMN_NAME R_COLUMN_NAME FROM ALL_CONSTRAINTS AC JOIN ALL_CONS_COLUMNS CC ON AC.CONSTRAINT_NAME = CC.CONSTRAINT_NAME JOIN ALL_CONS_COLUMNS RCC ON AC.R_OWNER = RCC.OWNER AND AC.R_CONSTRAINT_NAME = RCC.CONSTRAINT_NAME WHERE AC.CONSTRAINT_TYPE='R' ");
	if (schema && zend_is_true(schema)) {
		PHALCON_SCONCAT_SVSVS(&sql, "AND AC.OWNER='", schema, "' AND AC.TABLE_NAME = '", table, "'");
	} else {
		PHALCON_SCONCAT_SVS(&sql, "AND AC.TABLE_NAME = '", table, "'");
	}

	RETURN_CTORW(&sql);
}

/**
 * Generates the SQL to describe the table creation options
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, tableOptions){

	zval *table, *schema = NULL;

	phalcon_fetch_params(0, 1, 1, &table, &schema);

	RETURN_EMPTY_STRING();
}

/**
 * Transform an intermediate representation for a schema/table into a database system valid expression
 *
 * @param array $table
 * @param string $escapeChar
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, getSqlTable){

	zval *table, *escape = NULL, escape_char = {}, table_name = {}, sql_table = {}, schema_name = {}, sql_schema = {}, alias_name = {}, sql_table_alias = {};

	phalcon_fetch_params(0, 1, 1, &table, &escape);

	if (!escape || Z_TYPE_P(escape) == IS_NULL) {
		 phalcon_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_NOISY);
	} else {
		PHALCON_CPY_WRT(&escape_char, escape);
	}

	if (Z_TYPE_P(table) == IS_ARRAY) {
		/** 
		 * The index '0' is the table name
		 */
		phalcon_array_fetch_long(&table_name, table, 0, PH_NOISY);
		if (PHALCON_GLOBAL(db).escape_identifiers) {
			PHALCON_CONCAT_VVV(&sql_table, &escape_char, &table_name, &escape_char);
		} else {
			PHALCON_CPY_WRT_CTOR(&sql_table, &table_name);
		}

		/** 
		 * The index '1' is the schema name
		 */
		phalcon_array_fetch_long(&schema_name, table, 1, PH_NOISY);
		if (Z_TYPE(schema_name) != IS_NULL) {
			if (PHALCON_GLOBAL(db).escape_identifiers) {
				PHALCON_CONCAT_VVVSV(&sql_schema, &escape_char, &schema_name, &escape_char, ".", &sql_table);
			} else {
				PHALCON_CONCAT_VSV(&sql_schema, &schema_name, ".", &sql_table);
			}
		} else {
			PHALCON_CPY_WRT_CTOR(&sql_schema, &sql_table);
		}

		/** 
		 * The index '2' is the table alias
		 */
		if (phalcon_array_isset_fetch_long(&alias_name, table, 2)) {
			if (PHALCON_GLOBAL(db).escape_identifiers) {
				PHALCON_CONCAT_VSVVV(&sql_table_alias, &sql_schema, " ", &escape_char, &alias_name, &escape_char);
			} else {
				PHALCON_CONCAT_VSV(&sql_table_alias, &sql_schema, " ", &alias_name);
			}
		} else {
			PHALCON_CPY_WRT_CTOR(&sql_table_alias, &sql_schema);
		}

		RETURN_CTORW(&sql_table_alias);
	}

	if (PHALCON_GLOBAL(db).escape_identifiers) {
		PHALCON_CONCAT_VVV(&sql_table, &escape_char, table, &escape_char);
		RETURN_CTORW(&sql_table);
	}

	RETURN_CTORW(table);
}

/**
 * Generates the SQL for LIMIT clause
 *
 *<code>
 * $sql = $dialect->limit('SELECT * FROM robots', 10);
 * echo $sql; // SELECT * FROM robots LIMIT 10
 *</code>
 *
 * @param string $sqlQuery
 * @param int $number
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, limit){

	zval *sql_query, *number, limit = {}, sql_limit = {};

	phalcon_fetch_params(0, 2, 0, &sql_query, &number);

	if (phalcon_is_numeric(number)) {
		ZVAL_LONG(&limit, phalcon_get_intval(number));

		PHALCON_CONCAT_VSV(&sql_limit, sql_query, " LIMIT ", &limit);
		RETURN_CTORW(&sql_limit);
	}

	RETURN_CTORW(sql_query);
}

/**
 * Builds a SELECT statement
 *
 * @param array $definition
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, select){

	zval *definition, escape_char = {}, tables = {}, columns = {}, selected_columns = {}, *column, columns_sql = {}, selected_tables = {};
	zval *table, tables_sql = {}, distinct = {}, sql = {}, joins = {}, *join, where_conditions = {}, where_expression = {}, group_fields = {}, group_items = {};
	zval *group_field, group_sql = {}, group_clause = {}, having_conditions = {}, having_expression = {};
	zval order_fields = {}, order_items = {}, *order_item, order_sql = {}, limit_value = {};
	zval number = {}, offset = {}, tmp1 = {}, tmp2 = {}, ini_range = {}, end_range = {}, sql_limit = {};

	phalcon_fetch_params(0, 1, 0, &definition)

	if (Z_TYPE_P(definition) != IS_ARRAY) {
		PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "Invalid SELECT definition");
		return;
	}
	if (!phalcon_array_isset_fetch_str(&tables, definition, SL("tables"))) {
		PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "The index 'tables' is required in the definition array");
		return;
	}

	if (!phalcon_array_isset_fetch_str(&columns, definition, SL("columns"))) {
		PHALCON_THROW_EXCEPTION_STRW(phalcon_db_exception_ce, "The index 'columns' is required in the definition array");
		return;
	}

	if (PHALCON_GLOBAL(db).escape_identifiers) {
		phalcon_return_property(&escape_char, getThis(), SL("_escapeChar"));
	}

	if (Z_TYPE_P(&columns) == IS_ARRAY) {
		array_init(&selected_columns);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&columns), column) {
			zval column_item = {}, column_sql = {}, column_domain = {}, column_domain_sql = {}, column_alias = {}, column_alias_sql = {};
			/**
			 * Escape column name
			 */
			phalcon_array_fetch_long(&column_item, column, 0, PH_NOISY);
			if (Z_TYPE_P(&column_item) == IS_ARRAY) {
				PHALCON_CALL_METHODW(&column_sql, getThis(), "getsqlexpression", &column_item, &escape_char);
			} else if (PHALCON_IS_STRING(&column_item, "*")) {
				PHALCON_CPY_WRT_CTOR(&column_sql, &column_item);
			} else if (PHALCON_GLOBAL(db).escape_identifiers) {
				PHALCON_CONCAT_VVV(&column_sql, &escape_char, &column_item, &escape_char);
			} else {
				PHALCON_CPY_WRT_CTOR(&column_sql, &column_item);
			}

			/**
			 * Escape column domain
			 */
			if (phalcon_array_isset_fetch_long(&column_domain, column, 1)) {
				if (zend_is_true(&column_domain)) {
					if (PHALCON_GLOBAL(db).escape_identifiers) {
						PHALCON_CONCAT_VVVSV(&column_domain_sql, &escape_char, &column_domain, &escape_char, ".", &column_sql);
					} else {
						PHALCON_CONCAT_VSV(&column_domain_sql, &column_domain, ".", &column_sql);
					}
				} else {
					PHALCON_CPY_WRT_CTOR(&column_domain_sql, &column_sql);
				}
			} else {
				PHALCON_CPY_WRT_CTOR(&column_domain_sql, &column_sql);
			}

			/**
			 * Escape column alias
			 */
			if (phalcon_array_isset_fetch_long(&column_alias, column, 2)) {
				if (zend_is_true(&column_alias)) {
					if (PHALCON_GLOBAL(db).escape_identifiers) {
						PHALCON_CONCAT_VSVVV(&column_alias_sql, &column_domain_sql, " ", &escape_char, &column_alias, &escape_char);
					} else {
						PHALCON_CONCAT_VSV(&column_alias_sql, &column_domain_sql, " ", &column_alias);
					}
				} else {
					PHALCON_CPY_WRT_CTOR(&column_alias_sql, &column_domain_sql);
				}
			} else {
				PHALCON_CPY_WRT_CTOR(&column_alias_sql, &column_domain_sql);
			}

			phalcon_array_append(&selected_columns, &column_alias_sql, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		phalcon_fast_join_str(&columns_sql, SL(", "), &selected_columns);
	} else {
		PHALCON_CPY_WRT_CTOR(&columns_sql, &columns);
	}

	/**
	 * Check and escape tables
	 */
	if (Z_TYPE_P(&tables) == IS_ARRAY) {
		array_init(&selected_tables);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&tables), table) {
			zval sql_table = {};
			PHALCON_CALL_METHODW(&sql_table, getThis(), "getsqltable", table, &escape_char);
			phalcon_array_append(&selected_tables, &sql_table, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		phalcon_fast_join_str(&tables_sql, SL(", "), &selected_tables);
	} else {
		PHALCON_CPY_WRT_CTOR(&tables_sql, &tables);
	}

	if (phalcon_array_isset_fetch_str(&distinct, definition, SL("distinct"))) {
		assert(Z_TYPE_P(&distinct) == IS_LONG);
		if (Z_LVAL_P(&distinct) == 0) {
			ZVAL_STRING(&sql, "SELECT ALL ");
		}
		else if (Z_LVAL_P(&distinct) == 1) {
			ZVAL_STRING(&sql, "SELECT DISTINCT ");
		}
		else {
			ZVAL_STRING(&sql, "SELECT ");
		}
	}
	else {
		ZVAL_STRING(&sql, "SELECT ");
	}

	PHALCON_SCONCAT_VSV(&sql, &columns_sql, " FROM ", &tables_sql);

	/**
	 * Check for joins
	 */
	if (phalcon_array_isset_fetch_str(&joins, definition, SL("joins"))) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&joins), join) {
			zval type = {}, source = {}, sql_table = {}, sql_join = {}, join_conditions_array = {}, join_expressions = {}, join_conditions = {}, *join_condition;

			phalcon_array_fetch_str(&type, join, SL("type"), PH_NOISY);
			phalcon_array_fetch_str(&source, join, SL("source"), PH_NOISY);

			PHALCON_CALL_METHODW(&sql_table, getThis(), "getsqltable", &source, &escape_char);
			phalcon_array_append(&selected_tables, &sql_table, PH_COPY);

			PHALCON_CONCAT_SVSV(&sql_join, " ", &type, " JOIN ", &sql_table);

			/**
			 * Check if the join has conditions
			 */
			if (phalcon_array_isset_fetch_str(&join_conditions_array, join, SL("conditions"))) {
				if (phalcon_fast_count_ev(&join_conditions_array)) {
					array_init(&join_expressions);

					ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&join_conditions_array), join_condition) {
						zval join_expression = {};
						PHALCON_CALL_METHODW(&join_expression, getThis(), "getsqlexpression", join_condition, &escape_char);
						phalcon_array_append(&join_expressions, &join_expression, PH_COPY);
					} ZEND_HASH_FOREACH_END();

					phalcon_fast_join_str(&join_conditions, SL(" AND "), &join_expressions);
					PHALCON_SCONCAT_SV(&sql_join, " ON ", &join_conditions);
				}
			}
			phalcon_concat_self(&sql, &sql_join);
		} ZEND_HASH_FOREACH_END();

	}

	/**
	 * Check for a WHERE clause
	 */
	if (phalcon_array_isset_fetch_str(&where_conditions, definition, SL("where"))) {
		if (Z_TYPE_P(&where_conditions) == IS_ARRAY) {
			PHALCON_CALL_METHODW(&where_expression, getThis(), "getsqlexpression", &where_conditions, &escape_char);
			PHALCON_SCONCAT_SV(&sql, " WHERE ", &where_expression);
		} else {
			PHALCON_SCONCAT_SV(&sql, " WHERE ", &where_conditions);
		}
	}

	/**
	 * Check for a GROUP clause
	 */
	if (phalcon_array_isset_fetch_str(&group_fields, definition, SL("group"))) {
		array_init(&group_items);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&group_fields), group_field) {
			zval group_expression = {};

			PHALCON_CALL_METHODW(&group_expression, getThis(), "getsqlexpression", group_field, &escape_char);
			phalcon_array_append(&group_items, &group_expression, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		phalcon_fast_join_str(&group_sql, SL(", "), &group_items);

		PHALCON_CONCAT_SV(&group_clause, " GROUP BY ", &group_sql);
		phalcon_concat_self(&sql, &group_clause);
	}

	/**
	 * Check for a HAVING clause
	 */
	if (phalcon_array_isset_fetch_str(&having_conditions, definition, SL("having"))) {
		PHALCON_CALL_METHODW(&having_expression, getThis(), "getsqlexpression", &having_conditions, &escape_char);
		PHALCON_SCONCAT_SV(&sql, " HAVING ", &having_expression);
	}

	/**
	 * Check for a ORDER clause
	 */
	if (phalcon_array_isset_fetch_str(&order_fields, definition, SL("order"))) {
		array_init(&order_items);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&order_fields), order_item) {
			zval order_expression = {}, order_sql_item = {}, sql_order_type = {}, order_sql_item_type = {};

			phalcon_array_fetch_long(&order_expression, order_item, 0, PH_NOISY);

			PHALCON_CALL_METHODW(&order_sql_item, getThis(), "getsqlexpression", &order_expression, &escape_char);

			/**
			 * In the numeric 1 position could be a ASC/DESC clause
			 */
			if (phalcon_array_isset_fetch_long(&sql_order_type, order_item, 1)) {
				PHALCON_CONCAT_VSV(&order_sql_item_type, &order_sql_item, " ", &sql_order_type);
			} else {
				PHALCON_CPY_WRT_CTOR(&order_sql_item_type, &order_sql_item);
			}
			phalcon_array_append(&order_items, &order_sql_item_type, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		phalcon_fast_join_str(&order_sql, SL(", "), &order_items);
		PHALCON_SCONCAT_SV(&sql, " ORDER BY ", &order_sql);
	}

    /**
	 * Oracle does not implement the LIMIT clause as some RDBMS do.
	 * We have to simulate it with subqueries and ROWNUM.
	 * Unfortunately because we use the column wildcard "*",
	 * this puts an extra column into the query result set.
	 */
	if (phalcon_array_isset_fetch_str(&limit_value, definition, SL("limit"))) {
		if (likely(Z_TYPE_P(&limit_value) == IS_ARRAY)) {
			if (likely(phalcon_array_isset_fetch_str(&number, &limit_value, SL("number")))) {
				phalcon_array_fetch_str(&tmp1, &number, SL("value"), PH_NOISY);

				if (phalcon_array_isset_fetch_str(&offset, &limit_value, SL("offset"))) {
					phalcon_array_fetch_str(&tmp2, &offset, SL("value"), PH_NOISY);
				} else {
					ZVAL_LONG(&tmp2, 0);
				}

				phalcon_add_function(&ini_range, &tmp2, &PHALCON_GLOBAL(z_one));
				phalcon_add_function(&end_range, &tmp2, &tmp1);

				PHALCON_SCONCAT_SVSVSV(&sql_limit,"SELECT Z2.* FROM (SELECT Z1.*, ROWNUM DB_ROWNUM FROM ( ", &sql, " ) Z1 ) Z2 WHERE Z2.DB_ROWNUM BETWEEN ", &ini_range, " AND ",  &end_range);
				PHALCON_CPY_WRT_CTOR(&sql, &sql_limit);
			}
		} else {
			ZVAL_LONG(&ini_range, 1);

			PHALCON_CPY_WRT_CTOR(&end_range, &limit_value);

			PHALCON_SCONCAT_SVSVSV(&sql_limit,"SELECT Z2.* FROM (SELECT Z1.*, ROWNUM DB_ROWNUM FROM ( ", &sql, " ) Z1 ) Z2 WHERE Z2.DB_ROWNUM BETWEEN ", &ini_range, " AND ", &end_range);
			PHALCON_CPY_WRT_CTOR(&sql, &sql_limit);
		}
	}

	RETURN_CTORW(&sql);
}

/**
 * Checks whether the platform supports savepoints
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, supportsSavepoints){


	RETURN_FALSE;
}

/**
 * Checks whether the platform supports releasing savepoints.
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Dialect_Oracle, supportsReleaseSavepoints){


	RETURN_FALSE;
}

