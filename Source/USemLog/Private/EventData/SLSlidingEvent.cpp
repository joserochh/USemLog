// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "EventData/SLSlidingEvent.h"
#include "OwlExperimentStatics.h"

	// Default constructor
FSLSlidingEvent::FSLSlidingEvent()
{
}

// Constructor with initialization
FSLSlidingEvent::FSLSlidingEvent(const FString& InId,
	const float InStart,
	const float InEnd,
	const uint32 InSupportedObjId,
	const FString& InSupportedObjSemId,
	const FString& InSupportedObjClass,
	const uint32 InSupportingObjId,
	const FString& InSupportingObjSemId,
	const FString& InSupportingObjClass) :
	ISLEvent(InId, InStart, InEnd),
	SupportedObjId(InSupportedObjId),
	SupportedObjSemId(InSupportedObjSemId),
	SupportedObjClass(InSupportedObjClass),
	SupportingObjId(InSupportingObjId),
	SupportingObjSemId(InSupportingObjSemId),
	SupportingObjClass(InSupportingObjClass)
{
}

// Constructor with initialization without end time
FSLSlidingEvent::FSLSlidingEvent(const FString& InId,
	const float InStart,
	const uint32 InSupportedObjId,
	const FString& InSupportedObjSemId,
	const FString& InSupportedObjClass,
	const uint32 InSupportingObjId,
	const FString& InSupportingObjSemId,
	const FString& InSupportingObjClass) :
	ISLEvent(InId, InStart),
	SupportedObjId(InSupportedObjId),
	SupportedObjSemId(InSupportedObjSemId),
	SupportedObjClass(InSupportedObjClass),
	SupportingObjId(InSupportingObjId),
	SupportingObjSemId(InSupportingObjSemId),
	SupportingObjClass(InSupportingObjClass)
{
}

/* Begin ISLEvent interface */
// Get an owl representation of the event
FOwlNode FSLSlidingEvent::ToOwlNode() const
{
	// Create the contact event node
	FOwlNode EventIndividual = FOwlExperimentStatics::CreateEventIndividual(
		"log", Id, "SupportedBySituation");
	EventIndividual.AddChildNode(FOwlExperimentStatics::CreateStartTimeProperty("log", Start));
	EventIndividual.AddChildNode(FOwlExperimentStatics::CreateEndTimeProperty("log", End));
	EventIndividual.AddChildNode(FOwlExperimentStatics::CreateIsSupportedProperty("log", SupportedObjSemId));
	EventIndividual.AddChildNode(FOwlExperimentStatics::CreateIsSupportingProperty("log", SupportingObjSemId));
	return EventIndividual;
}

// Add the owl representation of the event to the owl document
void FSLSlidingEvent::AddToOwlDoc(FOwlDoc* OutDoc)
{
	// Add timepoint and object individuals
	// We know that the document is of type FOwlExperiment,
	// we cannot use the safer dynamic_cast because RTTI is not enabled by default
	// if (FOwlEvents* EventsDoc = dynamic_cast<FOwlEvents*>(OutDoc))
	FOwlExperiment* EventsDoc = static_cast<FOwlExperiment*>(OutDoc);
	EventsDoc->AddTimepointIndividual(
		Start, FOwlExperimentStatics::CreateTimepointIndividual("log", Start));
	EventsDoc->AddTimepointIndividual(
		End, FOwlExperimentStatics::CreateTimepointIndividual("log", End));
	EventsDoc->AddObjectIndividual(
		SupportedObjId, FOwlExperimentStatics::CreateObjectIndividual("log", SupportedObjSemId, SupportedObjClass));
	EventsDoc->AddObjectIndividual(
		SupportingObjId, FOwlExperimentStatics::CreateObjectIndividual("log", SupportingObjSemId, SupportingObjClass));
	OutDoc->AddIndividual(ToOwlNode());
}

// Get event context data as string (ToString equivalent)
FString FSLSlidingEvent::Context() const
{
	return FString("SlidingEvent");
}

// Get the tooltip data
FString FSLSlidingEvent::Tooltip() const
{
	return FString("SlidingEvent");;
}
/* End ISLEvent interface */