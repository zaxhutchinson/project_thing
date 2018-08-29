/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zaxlib_templates.hpp
 * Author: zax
 *
 * Created on November 30, 2017, 4:34 PM
 */

#ifndef ZAXLIB_TEMPLATES_HPP
#define ZAXLIB_TEMPLATES_HPP

#include<vector>
#include<memory>
#include<list>
#include<queue>
#include<map>

template<typename T>
using vec = std::vector<T>;
template<typename T>
using uptr = std::unique_ptr<T>;
template<typename T>
using sptr = std::shared_ptr<T>;
template<typename T>
using wptr = std::weak_ptr<T>;
template<typename T>
using vec_uptr = std::vector<uptr<T>>;
template<typename T>
using vec_sptr = std::vector<sptr<T>>;
template<typename T>
using vec_wptr = std::vector<wptr<T>>;
template<typename T>
using list_uptr = std::list<uptr<T>>;
template<typename T>
using list_sptr = std::list<sptr<T>>;
template<typename T>
using list_wptr = std::list<wptr<T>>;
template<typename T>
using q_uptr = std::queue<uptr<T>>;
template<typename T>
using q_sptr = std::queue<sptr<T>>;
template<typename T, typename U>
using zmap = std::map<T, U>;


#endif /* ZAXLIB_TEMPLATES_HPP */

