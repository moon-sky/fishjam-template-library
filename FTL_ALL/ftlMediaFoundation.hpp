#ifndef FTL_MEDIA_FOUNDATION_HPP
#define FTL_MEDIA_FOUNDATION_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlMediaFoundation.h"
#endif

#include <Mferror.h>

namespace FTL
{
	CFMediaFoundationErrorInfo::CFMediaFoundationErrorInfo(HRESULT hr) : CFComErrorInfo(hr)
	{
	}

	LPCTSTR CFMediaFoundationErrorInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			switch(m_Info)
			{
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PLATFORM_NOT_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BUFFERTOOSMALL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDREQUEST);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDSTREAMNUMBER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDMEDIATYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NOTACCEPTING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NOT_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_REPRESENTATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_MORE_TYPES);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_SERVICE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNEXPECTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDNAME);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDTYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_FILE_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALIDINDEX);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_TIMESTAMP);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_SCHEME);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_BYTESTREAM_TYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_TIME_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_SAMPLE_TIMESTAMP);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_SAMPLE_DURATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_STREAM_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_UNAVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_RATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_THINNING_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_REVERSE_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_RATE_TRANSITION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RATE_CHANGE_PREEMPTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NOT_AVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_CLOCK);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_MULTIPLE_BEGIN);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MULTIPLE_BEGIN);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MULTIPLE_SUBSCRIBERS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TIMER_ORPHANED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STATE_TRANSITION_PENDING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_STATE_TRANSITION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNRECOVERABLE_ERROR_OCCURRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SAMPLE_HAS_TOO_MANY_BUFFERS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SAMPLE_NOT_WRITABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_KEY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BAD_STARTUP_VERSION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_CAPTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_POSITION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ATTRIBUTENOTFOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_TYPE_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_TYPE_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_EMPTY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_NOT_EMPTY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_VECTOR_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_VECTOR_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_OPERATION_CANCELLED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BYTESTREAM_NOT_SEEKABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_DISABLED_IN_SAFEMODE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CANNOT_PARSE_BYTESTREAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SOURCERESOLVER_MUTUALLY_EXCLUSIVE_FLAGS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MEDIAPROC_WRONGSTATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_THROUGHPUT_NOT_AVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_TOO_MANY_CLASSES);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_WOULDBLOCK);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_BITPUMP);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_OUTOFMEMORY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_RT_WORKQUEUE_CLASS_NOT_SPECIFIED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INSUFFICIENT_BUFFER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CANNOT_CREATE_SINK);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BYTESTREAM_UNKNOWN_LENGTH);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SESSION_PAUSEWHILESTOPPED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_ACTIVATE_REPLACED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_FORMAT_CHANGE_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_WORKQUEUE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_DRM_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNAUTHORIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_OUT_OF_RANGE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_CODEC_MERIT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_HW_MFT_FAILED_START_STREAMING);

				//ASF Parsing Informational Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_ASF_PARSEINPROGRESS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_PARSINGINCOMPLETE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_MISSINGDATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_INVALIDDATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_OPAQUEPACKET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_NOINDEX);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_OUTOFRANGE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_INDEXNOTLOADED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_TOO_MANY_PAYLOADS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_UNSUPPORTED_STREAM_TYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_DROPPED_PACKET);

				//Media Source Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_EVENTS_AVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_STATE_TRANSITION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_END_OF_STREAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SHUTDOWN);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MP3_NOTFOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MP3_OUTOFDATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MP3_NOTMP3);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MP3_NOTSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_DURATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_READ_ONLY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROPERTY_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MEDIA_SOURCE_NOT_STARTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MP3_BAD_CRC);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NOT_PROTECTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MEDIA_SOURCE_WRONGSTATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MEDIA_SOURCE_NO_STREAMS_SELECTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CANNOT_FIND_KEYFRAME_SAMPLE);

				//Network Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NETWORK_RESOURCE_FAILURE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_WRITE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_READ);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_REQUIRE_NETWORK);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_REQUIRE_ASYNC);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_BWLEVEL_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_STREAMGROUPS_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_MANUALSS_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INVALID_PRESENTATION_DESCRIPTOR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_CACHESTREAM_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_I_MANUAL_PROXY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_REQUIRE_INPUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_REDIRECT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_REDIRECT_TO_PROXY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_TOO_MANY_REDIRECTS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_TIMEOUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_CLIENT_CLOSE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_BAD_CONTROL_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INCOMPATIBLE_SERVER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_UNSAFE_URL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_CACHE_NO_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_EOL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_BAD_REQUEST);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INTERNAL_SERVER_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_SESSION_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_NOCONNECTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_CONNECTION_FAILURE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INCOMPATIBLE_PUSHSERVER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_SERVER_ACCESSDENIED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_PROXY_ACCESSDENIED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_CANNOTCONNECT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INVALID_PUSH_TEMPLATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_INVALID_PUSH_PUBLISHING_POINT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_BUSY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_RESOURCE_GONE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_ERROR_FROM_PROXY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_PROXY_TIMEOUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_SERVER_UNAVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_TOO_MUCH_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_SESSION_INVALID);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_OFFLINE_MODE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_UDP_BLOCKED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_UNSUPPORTED_CONFIGURATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NET_PROTOCOL_DISABLED);

				//WMContainer Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ALREADY_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BANDWIDTH_OVERRUN);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LATE_SAMPLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_FLUSH_NEEDED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_PROFILE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INDEX_NOT_COMMITTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_INDEX);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CANNOT_INDEX_IN_PLACE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_MISSING_ASF_LEAKYBUCKET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_ASF_STREAMID);

				//Media Sink Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STREAMSINK_REMOVED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STREAMSINKS_OUT_OF_SYNC);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STREAMSINKS_FIXED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STREAMSINK_EXISTS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SAMPLEALLOCATOR_CANCELED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SAMPLEALLOCATOR_EMPTY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SINK_ALREADYSTOPPED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ASF_FILESINK_BITRATE_UNKNOWN);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SINK_NO_STREAMS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_SINK_NOT_FINALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_METADATA_TOO_LONG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SINK_NO_SAMPLES_PROCESSED);

				//Renderer Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_VIDEO_REN_NO_PROCAMP_HW);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_VIDEO_REN_NO_DEINTERLACE_HW);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_VIDEO_REN_COPYPROT_FAILED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_VIDEO_REN_SURFACE_NOT_SHARED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_VIDEO_DEVICE_LOCKED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NEW_VIDEO_DEVICE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_VIDEO_SAMPLE_AVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_AUDIO_PLAYBACK_DEVICE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_AUDIO_PLAYBACK_DEVICE_IN_USE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_AUDIO_PLAYBACK_DEVICE_INVALIDATED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_AUDIO_SERVICE_NOT_RUNNING);

				//Topology Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_INVALID_OPTIONAL_NODE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_CANNOT_FIND_DECRYPTOR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_CODEC_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_CANNOT_CONNECT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_INVALID_TIME_ATTRIBUTES);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_LOOPS_IN_TOPOLOGY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_MISSING_PRESENTATION_DESCRIPTOR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_MISSING_STREAM_DESCRIPTOR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_STREAM_DESCRIPTOR_NOT_SELECTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_MISSING_SOURCE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPO_SINK_ACTIVATES_UNSUPPORTED);

				//Timeline Error Events
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SEQUENCER_UNKNOWN_SEGMENT_ID);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_SEQUENCER_CONTEXT_CANCELED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_SOURCE_IN_CACHE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_SEQUENCER_SEGMENT_AT_END_OF_STREAM);

				//Transform errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_TYPE_NOT_SET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_STREAM_CHANGE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_INPUT_REMAINING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROFILE_MISSING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROFILE_INVALID_OR_CORRUPT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROFILE_TRUNCATED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_PID_NOT_RECOGNIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_VARIANT_TYPE_WRONG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_NOT_WRITEABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_ARRAY_VALUE_WRONG_NUM_DIM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_VALUE_SIZE_WRONG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_VALUE_OUT_OF_RANGE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_PROPERTY_VALUE_INCOMPATIBLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_NOT_POSSIBLE_FOR_CURRENT_OUTPUT_MEDIATYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_NOT_POSSIBLE_FOR_CURRENT_INPUT_MEDIATYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_NOT_POSSIBLE_FOR_CURRENT_MEDIATYPE_COMBINATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_CONFLICTS_WITH_OTHER_CURRENTLY_ENABLED_FEATURES);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_NEED_MORE_INPUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_NOT_POSSIBLE_FOR_CURRENT_SPKR_CONFIG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_TRANSFORM_DO_NOT_PROPAGATE_EVENT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_UNSUPPORTED_D3D_TYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_ASYNC_LOCKED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSFORM_CANNOT_INITIALIZE_ACM_DRIVER);

				//Content Protection errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LICENSE_INCORRECT_RIGHTS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LICENSE_OUTOFDATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LICENSE_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_DRM_HARDWARE_INCONSISTENT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_CONTENT_PROTECTION_MANAGER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LICENSE_RESTORE_NO_RIGHTS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BACKUP_RESTRICTED_LICENSE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_LICENSE_RESTORE_NEEDS_INDIVIDUALIZATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_PROTECTION_NOT_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_COMPONENT_REVOKED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRUST_DISABLED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_NO_ACTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_ACTION_ALREADY_SET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_DRM_HEADER_NOT_AVAILABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_DRM_ENCRYPTION_SCHEME_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_ACTION_MISMATCH);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_WMDRMOTA_INVALID_POLICY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_POLICY_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_OPL_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TOPOLOGY_VERIFICATION_FAILED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_SIGNATURE_VERIFICATION_FAILED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_DEBUGGING_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CODE_EXPIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_VERSION_TOO_LOW);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_RENEWAL_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_EXTENSIBLE_ENTRY_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_KERNEL_UNTRUSTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PEAUTH_UNTRUSTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NON_PE_PROCESS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_REBOOT_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_WAIT_FOR_POLICY_SET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_VIDEO_DISABLED_WITH_UNKNOWN_SOFTWARE_OUTPUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_INVALID_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_UNRECOGNIZED_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ALL_PROCESS_RESTART_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PROCESS_RESTART_REQUIRED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_USERMODE_UNTRUSTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PEAUTH_SESSION_NOT_STARTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PEAUTH_PUBLICKEY_REVOKED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_GRL_ABSENT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_PE_TRUSTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PE_UNTRUSTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PEAUTH_NOT_STARTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INCOMPATIBLE_SAMPLE_PROTECTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_PE_SESSIONS_MAXED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_HIGH_SECURITY_LEVEL_CONTENT_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TEST_SIGNED_COMPONENTS_NOT_ALLOWED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_UNSUPPORTED_ACTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_ERROR_PARSING_SAP_PARAMETERS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_POLICY_MGR_ACTION_OUTOFBOUNDS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_BAD_OPL_STRUCTURE_FORMAT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_UNRECOGNIZED_ANALOG_VIDEO_PROTECTION_GUID);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_PMP_HOST);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_OPL_DATA_NOT_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_UNRECOGNIZED_ANALOG_VIDEO_OUTPUT);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ITA_UNRECOGNIZED_DIGITAL_VIDEO_OUTPUT);

				//Clock errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CLOCK_INVALID_CONTINUITY_KEY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CLOCK_NO_TIME_SOURCE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CLOCK_STATE_ALREADY_SET);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_CLOCK_NOT_SIMPLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_S_CLOCK_STOPPED);

				//Quality Management errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_MORE_DROP_MODES);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_NO_MORE_QUALITY_LEVELS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_DROPTIME_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_QUALITYKNOB_WAIT_LONGER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_QM_INVALIDSTATE);

				//Transcode errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSCODE_NO_CONTAINERTYPE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSCODE_PROFILE_NO_MATCHING_STREAMS);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_TRANSCODE_NO_MATCHING_ENCODER);

				//HW Device Proxy errors
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ALLOCATOR_NOT_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ALLOCATOR_NOT_COMMITED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_ALLOCATOR_ALREADY_COMMITED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_STREAM_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_INVALID_STREAM_STATE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MF_E_HW_STREAM_NOT_CONNECTED);
				//HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), XXXXXXXXXXX);
			default:
				CFComErrorInfo comErrorInfo(m_Info);
				StringCchPrintf(m_bufInfo,_countof(m_bufInfo), TEXT("%s"),comErrorInfo.GetConvertedInfo());
				break;
			}
		}
		return m_bufInfo;
	}
}

#endif //FTL_MEDIA_FOUNDATION_HPP
