#include <sigmaengine/dapp_history/dapp_history_api.hpp>

namespace sigmaengine { namespace dapp_history {
   namespace detail {
      class dapp_history_api_impl
      {
         public:
            dapp_history_api_impl( sigmaengine::app::application& app ):_app( app ) {}
            map< uint32_t, applied_operation > get_dapp_history( string dapp_name, uint64_t from, uint32_t limit )const;
            map< uint32_t, applied_operation > get_nsta602_transfer_history( string dapp_name, string author, string unique_id, uint64_t from, uint32_t limit )const;
            map< uint32_t, applied_operation > get_dapp_operation_list( uint64_t from, uint32_t limit )const;
            sigmaengine::chain::database& database() { return *_app.chain_database(); }

         private:
            sigmaengine::app::application& _app;
      };

      
      map< uint32_t, applied_operation > dapp_history_api_impl::get_dapp_history( string dapp_name, uint64_t from, uint32_t limit )const  {
         auto _db = _app.chain_database();
         FC_ASSERT( limit <= 10000, "Limit of ${l} is greater than maxmimum allowed", ("l",limit) );
         FC_ASSERT( from >= limit, "From must be greater than limit" );
         const auto& idx = _db->get_index< dapp_history_index >().indices().get < by_dapp_name >();
         auto itr = idx.lower_bound( boost::make_tuple( dapp_name, from ) );
         auto end = idx.upper_bound( boost::make_tuple( dapp_name, std::max( int64_t(0), int64_t( itr->sequence ) - limit ) ) );

         map<uint32_t, applied_operation> result;
         while( itr != end )
         {
            result[itr->sequence] = _db->get(itr->op);
            ++itr;
         }
         return result;
      }

      map< uint32_t, applied_operation > dapp_history_api_impl::get_dapp_operation_list( uint64_t from, uint32_t limit )const
      {
         FC_ASSERT( limit <= 10000, "Limit of ${l} is greater than maxmimum allowed", ("l",limit) );
         FC_ASSERT( from >= limit, "From must be greater than limit" );

         auto _db = _app.chain_database();
      
         const auto & idx = _db->get_index< dapp_history_index >().indices().get< by_transaction >();
         
         auto itr = idx.lower_bound( from );
         auto end = idx.upper_bound( std::max( int64_t(0), int64_t(itr->all_sequence)-limit ) );

         map<uint32_t, applied_operation> result;
         while( itr != end )
         {
            result[itr->all_sequence] = _db->get(itr->op);
            ++itr;
         }
         return result;
      }

      map< uint32_t, applied_operation > dapp_history_api_impl::get_nsta602_transfer_history( string dapp_name, string author, string unique_id, uint64_t from, uint32_t limit )const  {
         auto _db = _app.chain_database();
         FC_ASSERT( limit <= 10000, "Limit of ${l} is greater than maxmimum allowed", ("l",limit) );
         FC_ASSERT( from >= limit, "From must be greater than limit" );
         const auto& idx = _db->get_index< nsta602_transfer_history_index >().indices().get < by_nsta602 >();
         auto itr = idx.lower_bound( boost::make_tuple( dapp_name, author, unique_id, from ) );
         auto end = idx.upper_bound( boost::make_tuple( dapp_name, author, unique_id, std::max( int64_t(0), int64_t( itr->sequence ) - limit ) ) );

         map<uint32_t, applied_operation> result;
         while( itr != end )
         {
            result[itr->sequence] = _db->get(itr->op);
            ++itr;
         }
         return result;
      }

   } //namespace details

   dapp_history_api::dapp_history_api( const sigmaengine::app::api_context& ctx ) {
      _my = std::make_shared< detail::dapp_history_api_impl >( ctx.app );
   }

   void dapp_history_api::on_api_startup() {}

   map< uint32_t, applied_operation > dapp_history_api::get_dapp_history( string dapp_name, uint64_t from, uint32_t limit ) const {
      return _my->database().with_read_lock( [ & ]() {
         return _my->get_dapp_history( dapp_name, from, limit );
      });
   }

   map< uint32_t, applied_operation > dapp_history_api::get_dapp_operation_list( uint64_t from, uint32_t limit )const
   {
      return _my->database().with_read_lock( [ & ]() {
         return _my->get_dapp_operation_list( from, limit );
      });
   }

   map< uint32_t, applied_operation > dapp_history_api::get_nsta602_transfer_history( string dapp_name, string author, string unique_id, uint64_t from, uint32_t limit ) const {
      return _my->database().with_read_lock( [ & ]() {
         return _my->get_nsta602_transfer_history( dapp_name, author, unique_id, from, limit );
      });
   }

} } //namespace sigmaengine::dapp_history
