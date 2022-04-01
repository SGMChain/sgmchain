#pragma once

#include <sigmaengine/app/plugin.hpp>

#include <sigmaengine/chain/sigmaengine_object_types.hpp>
#include <sigmaengine/protocol/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace sigmaengine { namespace dapp_history {
   using namespace std;
   using namespace sigmaengine::chain;
   using namespace boost::multi_index;
   using namespace sigmaengine::protocol;

   struct strcmp_less
   {
      bool operator()(const shared_string& a, const shared_string& b)const
      {
         return less(a.c_str(), b.c_str());
      }

      bool operator()(const shared_string& a, const string& b)const
      {
         return less(a.c_str(), b.c_str());
      }

      bool operator()(const string& a, const shared_string& b)const
      {
         return less(a.c_str(), b.c_str());
      }

   private:
      inline bool less(const char* a, const char* b)const
      {
         return std::strcmp(a, b) < 0;
      }
   };

   struct strcmp_equal
   {
      bool operator()(const shared_string& a, const string& b)
      {
         return a.size() == b.size() || std::strcmp(a.c_str(), b.c_str()) == 0;
      }
   };

   enum dapp_history_by_key_object_type
   {
      dapp_history_object_type               = (DAPP_HISTORY_SPACE_ID << 8),
      nsta602_transfer_history_object_type   = (DAPP_HISTORY_SPACE_ID << 8) + 1
   };

   class dapp_history_object : public object< dapp_history_object_type, dapp_history_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_history_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type                 id;

         dapp_name_type          dapp_name;
         uint32_t                sequence = 0;
         uint32_t                all_sequence = 0;
         operation_id_type       op;
   };

   typedef oid< dapp_history_object > dapp_history_id_type;

   class nsta602_transfer_history_object : public object< nsta602_transfer_history_object_type, nsta602_transfer_history_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         nsta602_transfer_history_object( Constructor&& c, allocator< Allocator > a )
         :unique_id(a)
         {
            c( *this );
         }

         id_type                 id;

         dapp_name_type          dapp_name;
         account_name_type       author;
         shared_string           unique_id;

         uint32_t                sequence = 0;
         operation_id_type       op;
   };

   typedef oid< nsta602_transfer_history_object > nsta602_transfer_history_id_type;

   struct by_id;
   struct by_dapp_name;
   struct by_transaction;
   struct by_nsta602;

   typedef multi_index_container<
      dapp_history_object,
      indexed_by<
         ordered_unique< tag< by_id >, 
            member< dapp_history_object, dapp_history_id_type, &dapp_history_object::id > 
         >,
         ordered_unique< tag< by_dapp_name >,
            composite_key< dapp_history_object,
               member< dapp_history_object, dapp_name_type, &dapp_history_object::dapp_name >,
               member< dapp_history_object, uint32_t, &dapp_history_object::sequence >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::greater< uint32_t > >
         >,
         ordered_unique< tag< by_transaction >,
            composite_key< dapp_history_object,
               member< dapp_history_object, uint32_t, &dapp_history_object::all_sequence >
            >,
            composite_key_compare< std::greater< uint32_t > >
         >
      >,
      allocator< dapp_history_object >
   > dapp_history_index;

   typedef multi_index_container<
      nsta602_transfer_history_object,
      indexed_by<
         ordered_unique< tag< by_id >, 
            member< nsta602_transfer_history_object, nsta602_transfer_history_id_type, &nsta602_transfer_history_object::id > 
         >,
         ordered_unique< tag< by_nsta602 >,
            composite_key< nsta602_transfer_history_object,
               member< nsta602_transfer_history_object, dapp_name_type, &nsta602_transfer_history_object::dapp_name >,
               member< nsta602_transfer_history_object, account_name_type, &nsta602_transfer_history_object::author >,
               member< nsta602_transfer_history_object, shared_string, &nsta602_transfer_history_object::unique_id >,
               member< nsta602_transfer_history_object, uint32_t, &nsta602_transfer_history_object::sequence >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less, std::greater< uint32_t > >
         >
      >,
      allocator< nsta602_transfer_history_object >
   > nsta602_transfer_history_index;
} } //namespace sigmaengine::dapp_history

FC_REFLECT( sigmaengine::dapp_history::dapp_history_object, (id)(dapp_name)(sequence)(op) )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp_history::dapp_history_object, sigmaengine::dapp_history::dapp_history_index )

FC_REFLECT( sigmaengine::dapp_history::nsta602_transfer_history_object, (id)(dapp_name)( author )( unique_id )(sequence)(op) )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp_history::nsta602_transfer_history_object, sigmaengine::dapp_history::nsta602_transfer_history_index )

