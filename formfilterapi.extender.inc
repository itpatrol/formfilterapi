<?php

/**
 * @file
 * Filter query extender and helper functions.
 */
class FormFilterAPI extends SelectQueryExtender {
  /**
   * The filtername.
   *
   * @var string
   */
  protected $filterName;

  /**
   * Sets up the filter name.
   *
   * @param $filterName
   *   A filter name.
   * @return
   *   The SearchQuery object.
   */
  public function processFilters($filterName) {
    $this->filterName = $filterName;

    $filters = formfilterapi_build_filter_query($this->filterName);
    
    if(is_array($filters['where'])){
      foreach($filters['where'] as $cond){
        foreach($cond as $field => $value){
          if(is_array($value)){
            switch ($value['op']){
              case 'like':
                $this->condition($field, "%".$value['value']."%", 'LIKE');
                break;
            }
          }else{
            $this->condition($field, $value);
          }
        }
      }
    }
    
    return $this;
  }
    
}
