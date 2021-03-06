// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Knowrob/SLKRWSClient.h"
#include "WebSocketsModule.h"


// Ctor
FSLKRWSClient::FSLKRWSClient()
{
}

// Dtor
FSLKRWSClient::~FSLKRWSClient()
{
}

// Set websocket conection parameters
void FSLKRWSClient::Init(const FString& InHost, int32 InPort, const FString& InProtocol)
{
	// TODO, why is this needed
	// Make sure the webscokets module is loaded
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	// Create the url and protocol entries
	TArray<FString> Protocols{ InProtocol };
	const FString Url = TEXT("ws://") + InHost + TEXT(":") + FString::FromInt(InPort);

	// Clear any previous connection
	if (WebSocket.IsValid())
	{
		if (WebSocket->IsConnected())
		{
			Disconnect();
		}
		Clear();
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(Url, Protocols);
	}
	else
	{
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(Url, Protocols);
	}

	// Bind WS delegates
	WebSocket->OnConnected().AddRaw(this, &FSLKRWSClient::HandleWebSocketConnected);
	WebSocket->OnConnectionError().AddRaw(this, &FSLKRWSClient::HandleWebSocketConnectionError);
	WebSocket->OnClosed().AddRaw(this, &FSLKRWSClient::HandleWebSocketConnectionClosed);
	WebSocket->OnRawMessage().AddRaw(this, &FSLKRWSClient::HandleWebSocketData);
}

// Start connection
void FSLKRWSClient::Connect()
{
	if (WebSocket.IsValid())
	{
		if (!WebSocket->IsConnected())
		{
			WebSocket->Connect();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s::%d WebSocket is already connected.."), *FString(__FUNCTION__), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s::%d WebSocket is not valid, call init first.."), *FString(__FUNCTION__), __LINE__);
	}
}

// Disconnect from the server.
void FSLKRWSClient::Disconnect()
{
	if (WebSocket.IsValid())
	{
		// Close connection
		WebSocket->Close();
	}

	// Clear any remaining messages
	ReceiveBuffer.Empty();
	MessageQueue.Empty();
}

// Clear the webscosket 
void FSLKRWSClient::Clear()
{
	if (WebSocket.IsValid())
	{
		// Close connection
		WebSocket->Close();

		// Unbind delgates
		WebSocket->OnConnected().RemoveAll(this);
		WebSocket->OnConnectionError().RemoveAll(this);
		WebSocket->OnClosed().RemoveAll(this);
		WebSocket->OnRawMessage().RemoveAll(this);

		// Reset shared pointer
		WebSocket.Reset();
	}

	// Clear any remaining messages
	ReceiveBuffer.Empty();
	MessageQueue.Empty();
}

// Called on connection
void FSLKRWSClient::HandleWebSocketConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("%s::%d::%.4f KR websocket client is connected.. "),
		*FString(__FUNCTION__), __LINE__, FPlatformTime::Seconds());

	// Trigger delegate
	OnConnection.ExecuteIfBound(true);
}

// Called on connection error
void FSLKRWSClient::HandleWebSocketConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("%s::%d::%.4f KR websocket client connection error: %s"),
		*FString(__FUNCTION__), __LINE__, FPlatformTime::Seconds(), *Error);

	// Trigger delegate
	OnConnection.ExecuteIfBound(false);
}

// Called on connection closed
void FSLKRWSClient::HandleWebSocketConnectionClosed(int32 Status, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Warning, TEXT("%s::%d::%.4f KR websocket client connection closed: Status=%d; Reason=%s; bWasClean=%d;"),
		*FString(__FUNCTION__), __LINE__, FPlatformTime::Seconds(), Status, *Reason, bWasClean);

	// Trigger delegate
	OnConnection.ExecuteIfBound(false);
}

// Called on new data (can be partial)
void FSLKRWSClient::HandleWebSocketData(const void* Data, SIZE_T Length, SIZE_T BytesRemaining)
{
	if (BytesRemaining == 0 && ReceiveBuffer.Num() == 0)
	{
		// Skip the temporary buffer when the entire frame arrives in a single message. (common case)
		HandleWebSocketFullData((const uint8*)Data, Length);
	}
	else
	{
		ReceiveBuffer.Append((const uint8*)Data, Length);
		if (BytesRemaining == 0)
		{
			HandleWebSocketFullData(ReceiveBuffer.GetData(), ReceiveBuffer.Num());
			ReceiveBuffer.Empty();
		}
	}
}

// Called when the full data has been received
void FSLKRWSClient::HandleWebSocketFullData(const uint8* Data, SIZE_T Length)
{
	UE_LOG(LogTemp, Warning, TEXT("%s::%d::%.4f KR websocket client new message enqueued.."),
		*FString(__FUNCTION__), __LINE__, FPlatformTime::Seconds());

	MessageQueue.Enqueue(std::string(Data, Data + Length));

	// Trigger delegate
	OnNewProcessedMsg.ExecuteIfBound();
}

// Send message via websocket
void FSLKRWSClient::SendResponse(const FString& StrToSend)
{
	FTCHARToUTF8 UTStr(*StrToSend);
	TArray<uint8> BinaryArr;
	AppendToUInt8Array(BinaryArr, (uint8*)UTStr.Get(), UTStr.Length(), true);
	BinaryArr.Add('\0');
	WebSocket->Send(BinaryArr.GetData(), BinaryArr.Num(), false);
}

// Append element to uint8 array and handle escape character
void FSLKRWSClient::AppendToUInt8Array(TArray<uint8>& Out, uint8* In, SIZE_T Length, bool bShouldEscape)
{
	if (bShouldEscape)
	{
		for (SIZE_T I = 0; I < Length; I++)
		{
			if (In[I] == ':' || In[I] == '\\' || In[I] == '\n' || In[I] == '\r')
			{
				Out.Add('\\');
			}
			Out.Add(In[I]);
		}
	}
	else
	{
		Out.Append(In, Length);
	}
}