#include "stdafx.h"
#include <Tcl_parser.h>

using namespace std;
using namespace tlib;

namespace tlib::parser
{
      ////////////////////////////////////////////////////////
      //
      //           class command_line_t::options_t
      //

      auto command_line_t::options_t::begin() noexcept -> command_line_t::options_t::const_iterator
      {
            return const_iterator(options_map.cbegin());
      }

      auto command_line_t::options_t::end() noexcept -> command_line_t::options_t::const_iterator
      {
            return const_iterator(options_map.cend());
      }

      auto command_line_t::options_t::cbegin() const noexcept -> command_line_t::options_t::const_iterator
      {
            return const_iterator(options_map.cbegin());
      }

      auto command_line_t::options_t::cend() const noexcept -> command_line_t::options_t::const_iterator
      {
            return const_iterator(options_map.cend());
      }

      const command_line_t::options_t::option_t& command_line_t::options_t::operator[](tlib::tstring_view str)
      {
            if (options_map.find(str.data()) != options_map.end())
            {
                  return options_map[str.data()];
            }
            else
            {
                  empty_option.name_option = str;
                  return empty_option;
            }
      }
      size_t command_line_t::options_t::size() const
      {
            return options_map.size();
      }

      bool command_line_t::options_t::empty() const
      {
            return options_map.empty();
      }

      ////////////////////////////////////////////////////////
      //
      //           class command_line_t::options_t::option_t::args_of_option
      //

      auto command_line_t::options_t::option_t::args_of_option::begin() noexcept -> command_line_t::options_t::option_t::args_of_option::const_iterator
      {
            return args.cbegin();
      }

      auto command_line_t::options_t::option_t::args_of_option::end() noexcept -> command_line_t::options_t::option_t::args_of_option::const_iterator
      {
            return args.cend();
      }

      auto command_line_t::options_t::option_t::args_of_option::cbegin() const noexcept -> command_line_t::options_t::option_t::args_of_option::const_iterator
      {
            return args.cbegin();
      }

      auto command_line_t::options_t::option_t::args_of_option::cend() const noexcept -> command_line_t::options_t::option_t::args_of_option::const_iterator
      {
            return args.cend();
      }

      command_line_t::options_t::option_t::args_of_option::operator bool() const
      {
            return args.size() != 0;
      }

      size_t command_line_t::options_t::option_t::args_of_option::size() const
      {
            return args.size();
      }

      const tlib::tstring& command_line_t::options_t::option_t::args_of_option::operator[](size_t n) const
      {
            if (n < size())
                  return args[n];
            else
                  return empty_string;
      }

      ////////////////////////////////////////////////////////
      //
      //           class command_line_t::options_t::option_t
      //

      command_line_t::options_t::option_t::operator bool() const
      {
            return is_set;
      }

      tlib::tstring command_line_t::options_t::option_t::name() const
      {
            return name_option;
      }

      ////////////////////////////////////////////////////////
      //
      //           class command_line_t::options_t::option_const_iter_t
      //

      command_line_t::options_t::option_const_iter_t&
           command_line_t::options_t::option_const_iter_t::operator = (const option_const_iter_t& iter) noexcept
      {
            if (this == &iter)
            {
                  return *this;
            }
            current_node = iter.current_node;
            return *this;
      }

      const command_line_t::options_t::option_t*
            command_line_t::options_t::option_const_iter_t::operator->()
      {
            return &(current_node->second);
      }


      const command_line_t::options_t::option_t&
            command_line_t::options_t::option_const_iter_t::operator*() noexcept
      {
            return current_node->second;
      }

      bool command_line_t::options_t::option_const_iter_t::operator==(const option_const_iter_t& iter) const noexcept
      {
            return current_node == iter.current_node;
      }

      bool command_line_t::options_t::option_const_iter_t::operator!=(const option_const_iter_t& iter) const noexcept
      {
            return current_node != iter.current_node;
      }

      command_line_t::options_t::option_const_iter_t&
            command_line_t::options_t::option_const_iter_t::operator++() noexcept
      {
            current_node++;
            return *this;
      }

      command_line_t::options_t::option_const_iter_t
            command_line_t::options_t::option_const_iter_t::operator++(int)
      {
            option_const_iter_t old_iter = *this;
            current_node++;
            return old_iter;
      }

      


      

}
