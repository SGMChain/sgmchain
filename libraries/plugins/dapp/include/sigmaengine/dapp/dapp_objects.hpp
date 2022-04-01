#pragma once

#include <sigmaengine/app/plugin.hpp>
#include <sigmaengine/chain/sigmaengine_object_types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace sigmaengine { namespace dapp {
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

   enum dapp_by_key_object_type
   {
      dapp_object_type                 = (DAPP_SPACE_ID << 8),
      dapp_comment_object_type         = (DAPP_SPACE_ID << 8) + 1,
      dapp_comment_vote_object_type    = (DAPP_SPACE_ID << 8) + 2,
      dapp_user_object_type            = (DAPP_SPACE_ID << 8) + 3,
      dapp_vote_object_type            = (DAPP_SPACE_ID << 8) + 4,
      dapp_trx_fee_vote_object_type    = (DAPP_SPACE_ID << 8) + 5,

      dapp_nsta602_object_type         = (DAPP_SPACE_ID << 8) + 6,
      dapp_nsta602_owner_object_type   = (DAPP_SPACE_ID << 8) + 7
   };

   class dapp_object : public object< dapp_object_type, dapp_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type                 id;
         dapp_name_type          dapp_name;
         account_name_type       owner;
         public_key_type         dapp_key;
         dapp_state_type         dapp_state = dapp_state_type::PENDING;    
         time_point_sec          created;
         time_point_sec          last_updated;
   };

   typedef oid< dapp_object > dapp_id_type;

   class dapp_comment_object : public object < dapp_comment_object_type, dapp_comment_object >
   {
      dapp_comment_object() = delete;

   public:
      template< typename Constructor, typename Allocator >
      dapp_comment_object(Constructor&& c, allocator< Allocator > a)
         :category(a), parent_permlink(a), permlink(a), title(a), body(a), json_metadata(a) //, beneficiaries(a)
      {
         c(*this);
      }

      id_type           id;
      dapp_name_type    dapp_name;

      shared_string     category;
      account_name_type parent_author;
      shared_string     parent_permlink;
      account_name_type author;
      shared_string     permlink;

      shared_string     title;
      shared_string     body;
      shared_string     json_metadata;
      time_point_sec    last_update;
      time_point_sec    created;
      time_point_sec    active; ///< the last time this post was "touched" by voting or reply

      uint16_t          depth = 0; ///< used to track max nested depth
      uint32_t          children = 0; ///< used to track the total number of children, grandchildren, etc...

      uint32_t          like_count = 0;
      uint32_t          dislike_count = 0;
         
      uint64_t          view_count = 0;

      id_type           root_comment;

      bool              allow_replies = true;      /// allows a post to disable replies.
      bool              allow_votes = true;      /// allows a post to receive votes;
   };

  /**
    * @ingroup object_index
    */
   typedef oid< dapp_comment_object > dapp_comment_id_type;

   class dapp_comment_vote_object : public object< dapp_comment_vote_object_type, dapp_comment_vote_object>
   {
   public:
      template< typename Constructor, typename Allocator >
      dapp_comment_vote_object(Constructor&& c, allocator< Allocator > a)
      {
         c(*this);
      }

      id_type                 id;
      dapp_name_type          dapp_name;
      account_id_type         voter;
      dapp_comment_id_type    comment;
      comment_vote_type       vote_type = comment_vote_type::LIKE;
      time_point_sec          last_update; ///< The time of the last update of the vote
   };
   
   typedef oid< dapp_comment_vote_object > dapp_comment_vote_id_type;

   class dapp_user_object : public object< dapp_user_object_type, dapp_user_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_user_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type                 id;
         dapp_id_type            dapp_id;
         dapp_name_type          dapp_name;
         account_id_type         account_id;
         account_name_type       account_name;
         time_point_sec          join_date_time;
   };

   typedef oid< dapp_user_object > dapp_user_id_type;

   class dapp_vote_object : public object< dapp_vote_object_type, dapp_vote_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_vote_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type                 id;
         dapp_name_type          dapp_name;
         account_name_type       voter;
         dapp_state_type         vote = dapp_state_type::PENDING;
         time_point_sec          last_update;
   };
   typedef oid< dapp_vote_object > dapp_vote_id_type;

   class dapp_trx_fee_vote_object : public object< dapp_trx_fee_vote_object_type, dapp_trx_fee_vote_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_trx_fee_vote_object( Constructor&& c, allocator< Allocator > a )
         {
            c( *this );
         }

         id_type                 id;
         account_name_type       voter;
         asset                   trx_fee = asset(0, SGT_SYMBOL);
         time_point_sec          last_update;
   };
   typedef oid< dapp_trx_fee_vote_object > dapp_trx_fee_vote_id_type;

   class dapp_nsta602_object : public object < dapp_nsta602_object_type, dapp_nsta602_object >
   {
      dapp_nsta602_object() = delete;
      public:
         template< typename Constructor, typename Allocator >
         dapp_nsta602_object( Constructor&& c, allocator< Allocator > a )
            :unique_id(a), info(a), json_meta(a) 
         {
            c( *this );
         }

         id_type                 id;
         dapp_name_type          dapp_name;
         account_name_type       author;
         shared_string           unique_id;

         uint64_t                init_supply;
         shared_string           info;
         shared_string           json_meta;
   };
   typedef oid< dapp_nsta602_object > dapp_nsta602_id_type;

   class dapp_nsta602_owner_object : public object < dapp_nsta602_owner_object_type, dapp_nsta602_owner_object >
   {
      public:
         template< typename Constructor, typename Allocator >
         dapp_nsta602_owner_object( Constructor&& c, allocator< Allocator > a )
            :unique_id(a)
         {
            c( *this );
         }

         id_type                 id;
         dapp_name_type          dapp_name;
         account_name_type       author;
         shared_string           unique_id;

         uint64_t                amount;
         account_name_type       owner;
   };
   typedef oid< dapp_nsta602_owner_object > dapp_nsta602_owner_id_type;

   struct by_name;
   struct by_owner;

   struct by_id;
   struct by_permlink; /// author, perm
   struct by_root;
   struct by_parent;
   struct by_active; /// parent_auth, active
   struct by_last_update; /// parent_auth, last_update
   struct by_created; /// parent_auth, last_update
   struct by_votes;
   struct by_responses;
   struct by_author_last_update;
   struct by_dapp_and_created;

   struct by_comment_voter;
   struct by_voter_comment;
   struct by_voter_last_update;
   struct by_vote_type;

   struct by_user_name;

   struct by_dapp_voter;

   struct by_voter;

   struct by_author;

   struct by_permlink_amount;
   struct by_owner_newest;
   struct by_author_newest;
   struct by_name_newest;
   struct by_permlink_newest;
   struct by_nsta602_owner;

   typedef multi_index_container <
      dapp_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_object, dapp_id_type, &dapp_object::id >
         >,
         ordered_unique < tag < by_name >,
            member < dapp_object, dapp_name_type, &dapp_object::dapp_name >
         >,
         ordered_non_unique < tag < by_owner >,
            member < dapp_object, account_name_type, &dapp_object::owner >
         >
      >,
      allocator < dapp_object >
   > dapp_index;

   typedef multi_index_container <
      dapp_comment_object,
      indexed_by <
      /// CONSENUSS INDICIES - used by evaluators
         ordered_unique < tag< by_id >
            , member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id > 
         >,
         ordered_unique < tag< by_permlink >, /// used by consensus to find posts referenced in ops
            composite_key < dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, account_name_type, &dapp_comment_object::author >,
               member< dapp_comment_object, shared_string, &dapp_comment_object::permlink >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less >
         >,
         ordered_unique < tag< by_root >,
            composite_key < dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::root_comment >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id >
            >
         >,
         ordered_unique< tag< by_parent >, /// used by consensus to find posts referenced in ops
            composite_key< dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, account_name_type, &dapp_comment_object::parent_author >,
               member< dapp_comment_object, shared_string, &dapp_comment_object::parent_permlink >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less, std::less< dapp_comment_id_type > >
         >
      /// NON_CONSENSUS INDICIES - used by APIs
#ifndef IS_LOW_MEM
         ,
         ordered_unique < tag< by_last_update >,
            composite_key < dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, account_name_type, &dapp_comment_object::parent_author >,
               member< dapp_comment_object, time_point_sec, &dapp_comment_object::last_update >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, std::greater< time_point_sec >, std::less< dapp_comment_id_type > >
         >,
         ordered_unique < tag< by_author_last_update >,
            composite_key < dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, account_name_type, &dapp_comment_object::author >,
               member< dapp_comment_object, time_point_sec, &dapp_comment_object::last_update >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, std::greater< time_point_sec >, std::less< dapp_comment_id_type > >
         >
#endif
         ,
         ordered_unique < tag< by_dapp_and_created >,
            composite_key< dapp_comment_object,
               member< dapp_comment_object, dapp_name_type, &dapp_comment_object::dapp_name >,
               member< dapp_comment_object, time_point_sec, &dapp_comment_object::created >,
               member< dapp_comment_object, dapp_comment_id_type, &dapp_comment_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::greater< time_point_sec >, std::less< dapp_comment_id_type > >
         >
      > ,
      allocator< dapp_comment_object >
   > dapp_comment_index;

   typedef multi_index_container <
      dapp_comment_vote_object,
      indexed_by <
         ordered_unique< tag< by_id >, 
            member< dapp_comment_vote_object, dapp_comment_vote_id_type, &dapp_comment_vote_object::id > >,
         ordered_unique< tag< by_comment_voter >,
            composite_key< dapp_comment_vote_object,
               member< dapp_comment_vote_object, dapp_comment_id_type, &dapp_comment_vote_object::comment >,
               member< dapp_comment_vote_object, account_id_type, &dapp_comment_vote_object::voter >,
               member< dapp_comment_vote_object, comment_vote_type, &dapp_comment_vote_object::vote_type >,
               member< dapp_comment_vote_object, time_point_sec, &dapp_comment_vote_object::last_update >
            >
         >,
         ordered_unique< tag< by_voter_comment >,
            composite_key< dapp_comment_vote_object,
               member< dapp_comment_vote_object, account_id_type, &dapp_comment_vote_object::voter>,
               member< dapp_comment_vote_object, dapp_comment_id_type, &dapp_comment_vote_object::comment>,
               member< dapp_comment_vote_object, comment_vote_type, &dapp_comment_vote_object::vote_type >,
               member< dapp_comment_vote_object, time_point_sec, &dapp_comment_vote_object::last_update >
            >
         >,
         ordered_unique< tag< by_voter_last_update >,
            composite_key< dapp_comment_vote_object,
               member< dapp_comment_vote_object, dapp_name_type, &dapp_comment_vote_object::dapp_name >,
               member< dapp_comment_vote_object, account_id_type, &dapp_comment_vote_object::voter>,
               member< dapp_comment_vote_object, time_point_sec, &dapp_comment_vote_object::last_update>,
               member< dapp_comment_vote_object, dapp_comment_id_type, &dapp_comment_vote_object::comment>
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_id_type >, std::greater< time_point_sec >, std::less< dapp_comment_id_type > >
         >,
         ordered_unique< tag< by_vote_type >,
            composite_key< dapp_comment_vote_object,
               member< dapp_comment_vote_object, comment_vote_type, &dapp_comment_vote_object::vote_type >,
               member< dapp_comment_vote_object, dapp_comment_id_type, &dapp_comment_vote_object::comment >,
               member< dapp_comment_vote_object, account_id_type, &dapp_comment_vote_object::voter >,
               member< dapp_comment_vote_object, time_point_sec, &dapp_comment_vote_object::last_update >
            >
         >
      > ,
      allocator< dapp_comment_vote_object >
   > dapp_comment_vote_index;

   typedef multi_index_container <
      dapp_user_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_user_object, dapp_user_id_type, &dapp_user_object::id >
         >,
         ordered_unique < tag < by_name >,
            composite_key< dapp_user_object,
               member < dapp_user_object, dapp_name_type, &dapp_user_object::dapp_name >,
               member < dapp_user_object, account_name_type, &dapp_user_object::account_name >,
               member < dapp_user_object, time_point_sec, &dapp_user_object::join_date_time >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, std::greater< time_point_sec > >
         >,
         ordered_non_unique < tag < by_user_name >,
            composite_key< dapp_user_object,
               member < dapp_user_object, account_name_type, &dapp_user_object::account_name >,
               member < dapp_user_object, dapp_name_type, &dapp_user_object::dapp_name >,
               member < dapp_user_object, time_point_sec, &dapp_user_object::join_date_time >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< dapp_name_type >, std::greater< time_point_sec > >
         >
      >,
      allocator < dapp_user_object >
   > dapp_user_index;

   typedef multi_index_container <
      dapp_vote_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_vote_object, dapp_vote_id_type, &dapp_vote_object::id >
         >,
         ordered_unique < tag < by_dapp_voter >,
            composite_key< dapp_vote_object,
               member < dapp_vote_object, dapp_name_type, &dapp_vote_object::dapp_name >,
               member < dapp_vote_object, account_name_type, &dapp_vote_object::voter >
            >
         >
      >,
      allocator < dapp_vote_object >
   > dapp_vote_index;

   typedef multi_index_container <
      dapp_trx_fee_vote_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_trx_fee_vote_object, dapp_trx_fee_vote_id_type, &dapp_trx_fee_vote_object::id >
         >,
         ordered_unique < tag < by_voter >,
            composite_key< dapp_trx_fee_vote_object,
               member < dapp_trx_fee_vote_object, account_name_type, &dapp_trx_fee_vote_object::voter >
            >
         >
      >,
      allocator < dapp_trx_fee_vote_object >
   > dapp_trx_fee_vote_index;

   typedef multi_index_container <
      dapp_nsta602_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_nsta602_object, dapp_nsta602_id_type, &dapp_nsta602_object::id >
         >,
         ordered_unique < tag < by_name >,
            composite_key< dapp_nsta602_object,
               member< dapp_nsta602_object, dapp_name_type, &dapp_nsta602_object::dapp_name >,
               member < dapp_nsta602_object, dapp_nsta602_id_type, &dapp_nsta602_object::id >
            >
            ,
            composite_key_compare< std::less< dapp_name_type >, std::less< dapp_nsta602_id_type > >
         >,
         ordered_unique < tag < by_name_newest >,
            composite_key< dapp_nsta602_object,
               member< dapp_nsta602_object, dapp_name_type, &dapp_nsta602_object::dapp_name >,
               member < dapp_nsta602_object, dapp_nsta602_id_type, &dapp_nsta602_object::id >
            >
            ,
            composite_key_compare< std::less< dapp_name_type >, std::greater< dapp_nsta602_id_type > >
         >,
         ordered_unique < tag < by_author >,
            composite_key< dapp_nsta602_object,
               member < dapp_nsta602_object, account_name_type, &dapp_nsta602_object::author >,
               member < dapp_nsta602_object, dapp_nsta602_id_type, &dapp_nsta602_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< dapp_nsta602_id_type > >
         >,
         ordered_unique < tag < by_author_newest >,
            composite_key< dapp_nsta602_object,
               member < dapp_nsta602_object, account_name_type, &dapp_nsta602_object::author >,
               member < dapp_nsta602_object, dapp_nsta602_id_type, &dapp_nsta602_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::greater< dapp_nsta602_id_type > >
         >,
         ordered_unique < tag < by_permlink >,
            composite_key< dapp_nsta602_object,
               member< dapp_nsta602_object, dapp_name_type, &dapp_nsta602_object::dapp_name >,
               member < dapp_nsta602_object, account_name_type, &dapp_nsta602_object::author >,
               member < dapp_nsta602_object, shared_string, &dapp_nsta602_object::unique_id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less >
         >
      >,
      allocator < dapp_nsta602_object >
   > dapp_nsta602_index;

   typedef multi_index_container <
      dapp_nsta602_owner_object,
      indexed_by <
         ordered_unique < tag < by_id >,
            member < dapp_nsta602_owner_object, dapp_nsta602_owner_id_type, &dapp_nsta602_owner_object::id >
         >,
         ordered_unique < tag < by_owner >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::owner >,
               member < dapp_nsta602_owner_object, dapp_nsta602_owner_id_type, &dapp_nsta602_owner_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< dapp_nsta602_owner_id_type > >
         >,
         ordered_unique < tag < by_owner_newest >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::owner >,
               member < dapp_nsta602_owner_object, dapp_nsta602_owner_id_type, &dapp_nsta602_owner_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::greater< dapp_nsta602_owner_id_type > >
         >,
         ordered_unique < tag < by_nsta602_owner >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::owner >,
               member< dapp_nsta602_owner_object, dapp_name_type, &dapp_nsta602_owner_object::dapp_name >,
               member < dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::author >,
               member < dapp_nsta602_owner_object, shared_string, &dapp_nsta602_owner_object::unique_id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less >
         >,
         ordered_unique < tag < by_permlink_amount >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, dapp_name_type, &dapp_nsta602_owner_object::dapp_name >,
               member < dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::author >,
               member < dapp_nsta602_owner_object, shared_string, &dapp_nsta602_owner_object::unique_id >,
               member < dapp_nsta602_owner_object, uint64_t, &dapp_nsta602_owner_object::amount >,
               member < dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::owner >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less, std::greater< uint64_t >, std::less< account_name_type > >
         >,
         ordered_unique < tag < by_permlink >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, dapp_name_type, &dapp_nsta602_owner_object::dapp_name >,
               member < dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::author >,
               member < dapp_nsta602_owner_object, shared_string, &dapp_nsta602_owner_object::unique_id >,
               member < dapp_nsta602_owner_object, dapp_nsta602_owner_id_type, &dapp_nsta602_owner_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less, std::less< dapp_nsta602_owner_id_type > >
         >,
         ordered_unique < tag < by_permlink_newest >,
            composite_key< dapp_nsta602_owner_object,
               member< dapp_nsta602_owner_object, dapp_name_type, &dapp_nsta602_owner_object::dapp_name >,
               member < dapp_nsta602_owner_object, account_name_type, &dapp_nsta602_owner_object::author >,
               member < dapp_nsta602_owner_object, shared_string, &dapp_nsta602_owner_object::unique_id >,
               member < dapp_nsta602_owner_object, dapp_nsta602_owner_id_type, &dapp_nsta602_owner_object::id >
            >,
            composite_key_compare< std::less< dapp_name_type >, std::less< account_name_type >, strcmp_less, std::greater< dapp_nsta602_owner_id_type > >
         >
      >,
      allocator < dapp_nsta602_owner_object >
   > dapp_nsta602_owner_index;

} } // namespace sigmaengine::dapp

FC_REFLECT( sigmaengine::dapp::dapp_object,
   ( id )
   ( dapp_name )
   ( owner )
   ( dapp_key )
   ( dapp_state )
   ( created )
   ( last_updated )
)

FC_REFLECT(sigmaengine::dapp::dapp_comment_object,
   ( id )
   ( dapp_name )
   ( author )
   ( permlink )
   ( category )
   ( parent_author )
   ( parent_permlink )
   ( title )
   ( body )
   ( json_metadata )
   ( last_update )
   ( created )
   ( active )
   ( depth )
   ( children )
   ( like_count )
   ( dislike_count )
   ( view_count )
   ( root_comment )
   ( allow_replies)
   ( allow_votes )
)

FC_REFLECT(sigmaengine::dapp::dapp_comment_vote_object,
   (id)
   (dapp_name)
   (voter)
   (comment)
   (vote_type)
   (last_update)
)

FC_REFLECT( sigmaengine::dapp::dapp_user_object,
   ( id )
   ( dapp_id )
   ( dapp_name )
   ( account_id )
   ( account_name )
   ( join_date_time )
)

FC_REFLECT( sigmaengine::dapp::dapp_vote_object,
   ( id )
   ( dapp_name )
   ( voter )
   ( vote )
   ( last_update )
)

FC_REFLECT( sigmaengine::dapp::dapp_trx_fee_vote_object,
   ( id )
   ( voter )
   ( trx_fee )
   ( last_update )
)

FC_REFLECT( sigmaengine::dapp::dapp_nsta602_object,
   ( id )
   ( dapp_name )
   ( author )
   ( unique_id )
   ( init_supply )
   ( info )
   ( json_meta )
)

FC_REFLECT( sigmaengine::dapp::dapp_nsta602_owner_object,
   ( id )
   ( dapp_name )
   ( author )
   ( unique_id )
   ( amount )
   ( owner )
)

CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_object, sigmaengine::dapp::dapp_index )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_comment_object, sigmaengine::dapp::dapp_comment_index)
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_comment_vote_object, sigmaengine::dapp::dapp_comment_vote_index)
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_user_object, sigmaengine::dapp::dapp_user_index )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_vote_object, sigmaengine::dapp::dapp_vote_index )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_trx_fee_vote_object, sigmaengine::dapp::dapp_trx_fee_vote_index )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_nsta602_object, sigmaengine::dapp::dapp_nsta602_index )
CHAINBASE_SET_INDEX_TYPE( sigmaengine::dapp::dapp_nsta602_owner_object, sigmaengine::dapp::dapp_nsta602_owner_index )


