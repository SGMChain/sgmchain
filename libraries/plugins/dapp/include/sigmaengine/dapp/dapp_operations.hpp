#pragma once

#include <sigmaengine/protocol/base.hpp>
#include <sigmaengine/app/plugin.hpp>

#include <sigmaengine/dapp/dapp_objects.hpp>

namespace sigmaengine { namespace dapp {
   using namespace std;
   using sigmaengine::protocol::base_operation;
   using namespace sigmaengine::chain;
   using namespace sigmaengine::protocol;

   class dapp_plugin;

   struct create_dapp_operation : base_operation
   {
      account_name_type                owner;
      dapp_name_type                   dapp_name;
      fc::optional< public_key_type >  dapp_key;  // dapp_key for owner

      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( owner );}
   };

   struct update_dapp_key_operation : base_operation
   {
      account_name_type                owner;
      dapp_name_type                   dapp_name;
      fc::optional< public_key_type >  dapp_key;  // dapp_key for owner
      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( owner );}
   };


   struct comment_dapp_operation : base_operation
   {
      dapp_name_type    dapp_name;
      account_name_type parent_author;
      string            parent_permlink;
      account_name_type author;
      string            permlink;
      string            title;
      string            body;
      string            json_metadata;

      void validate()const;
      void get_required_posting_authorities( flat_set< account_name_type >& a )const { a.insert( author );}
   };

   struct comment_vote_dapp_operation : public base_operation
   {
      dapp_name_type       dapp_name;
      account_name_type    voter;
      account_name_type    author;
      string               permlink;
      uint16_t             vote_type;

      void validate()const;
      void get_required_posting_authorities( flat_set< account_name_type >& a )const { a.insert( voter ); }
   };

   struct delete_comment_dapp_operation : public base_operation
   {
      dapp_name_type       dapp_name;
      account_name_type    author;
      string               permlink;

      void validate()const;
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert( author ); }
   };

   struct join_dapp_operation : base_operation
   {
      account_name_type                account_name;
      dapp_name_type                   dapp_name;

      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( account_name ); }
   };

   struct leave_dapp_operation : base_operation
   {
      account_name_type                account_name;
      dapp_name_type                   dapp_name;

      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( account_name ); }
   };

   struct vote_dapp_operation : base_operation
   {
      account_name_type                voter;
      dapp_name_type                   dapp_name;
      uint16_t                         vote;

      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( voter );}
   };

   struct vote_dapp_trx_fee_operation : base_operation
   {
      account_name_type             voter;
      asset                         trx_fee;

      void validate()const;
      void get_required_active_authorities( flat_set< account_name_type >& a )const { a.insert( voter );}
   };

   struct nsta602_create_operation : public base_operation
   {
      dapp_name_type       dapp_name;
      
      account_name_type    author;
      string               unique_id;
      int64_t              init_supply;
      string               info;
      string               json_meta;

      void                 validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert( author ); }
   };

   struct nsta602_transfer_operation : public base_operation
   {
      dapp_name_type       dapp_name;
      public_key_type      dapp_key;

      account_name_type    author;
      string               unique_id;

      account_name_type    from;
      account_name_type    to;

      uint32_t             amount;

      void                 validate()const;
      void                 get_required_authorities( std::vector< authority >& a )const { a.push_back( authority( 1, dapp_key, 1 ) ); }
   };

   struct nsta602_extransfer_operation : public base_operation
   {
      dapp_name_type       dapp_name;
      public_key_type      dapp_key;

      account_name_type    author;
      string               unique_id;

      account_name_type    from;
      account_name_type    to;

      uint32_t             amount;

      string               price;
      string               tx_id;
      string               memo;

      void                 validate()const;
      void                 get_required_authorities( std::vector< authority >& a )const { a.push_back( authority( 1, dapp_key, 1 ) ); }
   };
   
   typedef fc::static_variant< 
      create_dapp_operation
      , update_dapp_key_operation 
      , comment_dapp_operation
      , comment_vote_dapp_operation
      , delete_comment_dapp_operation 
      , join_dapp_operation
      , leave_dapp_operation
      , vote_dapp_operation
      , vote_dapp_trx_fee_operation
      , nsta602_create_operation
      , nsta602_transfer_operation
      , nsta602_extransfer_operation
   > dapp_operation;

   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, create_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, update_dapp_key )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, comment_dapp)
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, comment_vote_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, delete_comment_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, join_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, leave_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, vote_dapp )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, vote_dapp_trx_fee )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, nsta602_create )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, nsta602_transfer )
   DEFINE_PLUGIN_EVALUATOR( dapp_plugin, dapp_operation, nsta602_extransfer )

} } // namespace sigmaengine::dapp

FC_REFLECT( sigmaengine::dapp::create_dapp_operation,
   ( owner )
   ( dapp_name ) 
   ( dapp_key )
   )

FC_REFLECT( sigmaengine::dapp::update_dapp_key_operation,
   ( owner )
   ( dapp_name ) 
   ( dapp_key )
   )

FC_REFLECT( sigmaengine::dapp::comment_dapp_operation,
   ( dapp_name )
   ( parent_author )
   ( parent_permlink )
   ( author )
   ( permlink )
   ( title )
   ( body )
   ( json_metadata )
   )

FC_REFLECT( sigmaengine::dapp::comment_vote_dapp_operation,
   ( dapp_name )
   ( voter )
   ( author )
   ( permlink )
   ( vote_type )
   )

FC_REFLECT( sigmaengine::dapp::delete_comment_dapp_operation,
   ( dapp_name )
   ( author )
   ( permlink )
   )

FC_REFLECT( sigmaengine::dapp::join_dapp_operation,
   ( account_name )
   ( dapp_name ) 
   )

FC_REFLECT( sigmaengine::dapp::leave_dapp_operation,
   ( account_name )
   ( dapp_name ) 
   )

FC_REFLECT( sigmaengine::dapp::vote_dapp_operation,
   ( voter )
   ( dapp_name ) 
   ( vote )
   )

FC_REFLECT( sigmaengine::dapp::vote_dapp_trx_fee_operation,
   ( voter )
   ( trx_fee ) 
   )

FC_REFLECT( sigmaengine::dapp::nsta602_create_operation,
   ( dapp_name )
   ( author )
   ( unique_id )
   ( init_supply )
   ( info )
   ( json_meta )
   )

FC_REFLECT( sigmaengine::dapp::nsta602_transfer_operation,
   ( dapp_name )
   ( dapp_key )
   ( author )
   ( unique_id )
   ( from )
   ( to )
   ( amount )
   )


FC_REFLECT( sigmaengine::dapp::nsta602_extransfer_operation,
   ( dapp_name )
   ( dapp_key )
   ( author )
   ( unique_id )
   ( from )
   ( to )
   ( amount )
   ( price )
   ( tx_id )
   ( memo )
   )

DECLARE_OPERATION_TYPE( sigmaengine::dapp::dapp_operation )

FC_REFLECT_TYPENAME( sigmaengine::dapp::dapp_operation )

