<?php

/**
 * @file
 * Filter query extender and helper functions.
 */

/**
 * Query extender for form query condition.
 */
class FormFilterAPI extends SelectQueryExtender {
  /**
   * The filter name.
   *
   * @var string
   */
  protected $filterName;

  /**
   * Joins params.
   *
   * @var array
   */
  protected $joinParam = array();

  /**
   * Sets up the filter name.
   *
   * @param string $filter_name
   *   A filter name.
   *
   * @return object The SearchQuery object.
   *   The SearchQuery object.
   *
   * @throws FormFilterAPIException
   *   Error if join params count < 3.
   */
  public function processFilters($filter_name) {
    $this->filterName = $filter_name;

    $filters = formfilterapi_build_filter_query($this->filterName);

    if (is_array($filters['where'])) {
      foreach ($filters['where'] as $cond) {
        foreach ($cond as $field => $value) {
          if (is_array($value)) {
            switch ($value['op']) {
              case 'like':
                $this->condition($field, "%" . $value['value'] . "%", 'LIKE');
                break;

              case 'operation':
                list($operation, $operation_value) = explode(' ', $value['value']);
                $this->condition($field, $operation_value, $operation);
                break;
            }
          }
          else {
            $this->condition($field, $value);
          }
        }
      }
    }

    /**
     * Add to query left join:
     * @code
     * '#join' => array('left', array('users' => 'u'), array('%alias.uid', '=', 'pgt.uid')),
     * @endcode
     *
     * Inner join, table name = alias:
     * @code
     * '#join' => array('inner', 'users', array('%alias.uid', '=', 'pgt.uid')),
     * @endcode
     *
     *
     * Many joins:
     * @code
     * '#join' => array(
     *   array('inner', array('users' => 'u'), array('%alias.uid', '=', 'pgt.uid')),
     *   array('left', 'users', array('%alias.uid', '=', 'pgt.uid')),
     * ),
     * @endcode
     */
    if (is_array($filters['join']) && !empty($filters['join'])) {
      foreach ($filters['join'] as $joins) {
        if (is_string(reset($joins))) {
          $joins = array($joins);
        }

        foreach ($joins as $join) {
          if (count($join) == 3) {
            $this->joinParam = $join;

            $type = $this->getJoinType();
            $table = $this->getJoinTable();
            $alias = $this->getJoinTableAlias();
            $condition = $this->getJoinCondition();

            call_user_func_array(array($this, $type), array($table, $alias, $condition));
          }
          else {
            throw new FormFilterAPIException(t('Join parameters should have 3 elements in the array.'));
          }
        }
      }
    }

    return $this;
  }

  /**
   * Get join type.
   *
   * Avaiable type:
   * - leftJoin
   * - innerJoin
   *
   * @return string
   *   Join type.
   *
   * @see FormFilterAPI::processFilters()
   */
  protected function getJoinType() {
    $type = strtolower($this->joinParam[0]);

    if (in_array($type, array('left', 'leftjoin'))) {
      return 'leftJoin';
    }

    return 'innerJoin';
  }

  /**
   * Get name table for join.
   *
   * @return string
   *   Join table name.
   *
   * @see FormFilterAPI::processFilters()
   */
  protected function getJoinTable() {
    $table_name = is_array($this->joinParam[1]) ? key($this->joinParam[1]) : $this->joinParam[1];

    if (!$table_name) {
      $table_name = $this->getJoinTableAlias();
    }

    return $table_name;
  }

  /**
   * Get alias table.
   *
   * If alias is not specified, alias = table name.
   *
   * @return string
   *   Alias table name.
   *
   * @see FormFilterAPI::processFilters()
   */
  protected function getJoinTableAlias() {
    return is_array($this->joinParam[1]) ? reset($this->joinParam[1]) : $this->joinParam[1];
  }

  /**
   * Get join condition.
   *
   * @return string
   *   Condition string. Example: u.uid=t.uid.
   *
   * @throws FormFilterAPIException
   *   If join condition not have 3 elements if the array throw exception.
   *
   * @see FormFilterAPI::processFilters()
   */
  protected function getJoinCondition() {
    $condition = $this->joinParam[2];

    if (is_string($condition)) {
      return $condition;
    }

    if (count($condition) < 3) {
      throw new FormFilterAPIException(t('Join conditions must contain 3 elements of the array.'));
    }

    return implode(' ', $condition);
  }
}

/**
 * Exception thrown by FormFilterAPI().
 *
 * Error caused when checking parameters join.
 */
class FormFilterAPIException extends Exception {}
