//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>
//
//==============================================================================


#ifndef OBSERVER_EVENT_H_
#define OBSERVER_EVENT_H_

#include <vector>
#include <algorithm>

namespace SharedCore
{
	/*
	* An Event component which encapsulates the Observer pattern.
	* Observer pattern is one among popular 23 design patterns listed by Gang of Four (GoF).
	* The Event class's object can be used in a subject class to represent an event of the subject.
	* Make this object public so that subscribers can easily subscribe to this event
	*/
#if defined( _WIN32) || defined(__linux__)
	using namespace std;
#endif

	template<typename SourceType, typename EventArgType>
	class scEvent
	{
	public:
		/*
		* Event handler function pointer definition
		* source 		- Subject - the object which fired the event.
		* eventArg 	- The event argument
		* context		- Context information, which a subscriber needs to get with an event notification
		* 					Usually, this can be a pointer to the subscriber object itself.
		*/
		typedef void(*EventHandler)(SourceType *source, EventArgType eventArg, void* context);

	protected:
		/*
		* This inner class, for each EventHandler, stores the associated context information -
		* pointer. This context pointer can be used to pass additional information
		* from the point of subscription to the event handler function.
		* The best use of context pointer is to use the "this" pointer of subscriber itself.
		*/
		class SubscriberRecord
		{
		public:
			bool operator==( const SubscriberRecord& other ) const
			{
				return handlerProc == other.handlerProc;
			}
		private:
			EventHandler handlerProc; // The event handler function pointer
			void *handlerContext; // pointer to context information
			SubscriberRecord(EventHandler handler, void* context = 0) :
			handlerProc(handler), handlerContext(context)
			{
			}
			friend class scEvent;
		};
	protected:
		vector<SubscriberRecord> Subscribers;
		SourceType *eventSource;
	public:
		/*
		* Constructor - sets the Event source
		*/
		scEvent(SourceType *source) :
		  eventSource(source)
		  {
		  }

		  void SetSource( SourceType *source )
		  {
			  if ( eventSource == 0 )
			  {
				  eventSource = source;
			  }
		  }

		  /*
		  * Virtual destructor - perform clean up if any.
		  */
		  virtual ~scEvent()
		  {
		  }

		  /*
		  * Operator used to subscribe a handler C# style event subscription
		  */
		  void operator +=(EventHandler handler)
		  {
			  Subscribers.push_back(SubscriberRecord(handler));
		  }
		  /*
		  * Operator used to unsubscribe a handler C# style event subscription
		  */
		  void operator -=(EventHandler handler)
		  {
			  typename vector<SubscriberRecord>::iterator itr = find(Subscribers.begin(),
				  Subscribers.end(), SubscriberRecord(handler));
			  if (itr != Subscribers.end())
			  {
				  Subscribers.erase(itr);
			  }
		  }
		  /*
		  * Function used to subscribe a handler with optional context information
		  */
		  void Subscribe(EventHandler handler, void* context = 0)
		  {
			  Subscribers.push_back(SubscriberRecord(handler, context));
		  }

		  /*
		  * Function used to unsubscribe a handler with optional context information
		  */
		  void Unsubscribe(EventHandler handler, void* context = 0)
		  {
			  typename vector<SubscriberRecord>::iterator itr = find(Subscribers.begin(),
				  Subscribers.end(), SubscriberRecord(handler, context));
			  if (itr != Subscribers.end())
			  {
				  Subscribers.erase(itr);
			  }
		  }

		  /*
		  * Fire the event and notify all observers with event argument, -
		  * source and context information if any provided.
		  */
		  void FireEvent(EventArgType eventArg)
		  {
			  for (unsigned int i = 0; i < Subscribers.size(); i++)
			  {
				  Subscribers[i].handlerProc(eventSource, eventArg,
					  Subscribers[i].handlerContext);
			  }
		  }
	};

} // namespace SharedCore

#endif /* OBSERVER_EVENT_H_ */

