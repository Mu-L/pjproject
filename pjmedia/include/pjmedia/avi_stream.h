/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJMEDIA_AVI_STREAM_H__
#define __PJMEDIA_AVI_STREAM_H__

/**
 * @file avi_stream.h
 * @brief AVI file player.
 */
#include <pjmedia/port.h>


PJ_BEGIN_DECL


/**
 * @defgroup PJMEDIA_AVI_FILE_PLAY AVI File Player
 * @ingroup PJMEDIA_PORT
 * @brief Video and audio playback from AVI file
 * @{
 */

/**
 * AVI file player options.
 */
enum pjmedia_avi_file_player_option
{
    /**
     * Tell the file player to return NULL frame when the whole
     * file has been played.
     */
    PJMEDIA_AVI_FILE_NO_LOOP = 1,

    /**
     * Set the file player to permit independent playback of audio and
     * video streams without synchronization.
     */
    PJMEDIA_AVI_FILE_NO_SYNC = 2
};

/**
 * AVI stream data type.
 */
typedef pjmedia_port pjmedia_avi_stream;

/**
 * Opaque data type for AVI streams. AVI streams is a collection of
 * zero or more AVI stream.
 */
typedef struct pjmedia_avi_streams pjmedia_avi_streams;

struct pjmedia_avi_streams
{
    pj_pool_t       *pool;
    unsigned         num_streams;
    pjmedia_port   **streams;
};

/**
 * Create avi streams to play an AVI file. AVI player supports 
 * reading AVI file with uncompressed video format and 
 * 16 bit PCM or compressed G.711 A-law/U-law audio format.
 *
 * By default, avi streams will loop the file playback and synchronize
 * audio and video streams. To change this behavior, use the flags parameter.
 *
 * When synchronization is enabled, the file player will wait for all
 * media streams to reach the end of file before rewinding the file.
 *
 * @param pool          Pool to create the streams.
 * @param filename      File name to open.
 * @param flags         Avi streams creation flags, bitmask combination of
 *                      #pjmedia_avi_file_player_option.
 * @param p_streams     Pointer to receive the avi streams instance.
 *
 * @return              PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t)
pjmedia_avi_player_create_streams(pj_pool_t *pool,
                                  const char *filename,
                                  unsigned flags,
                                  pjmedia_avi_streams **p_streams);

/**
 * Get the number of AVI stream.
 *
 * @param streams       The AVI streams.
 *
 * @return              The number of AVI stream.
 */
PJ_DECL(unsigned)
pjmedia_avi_streams_get_num_streams(pjmedia_avi_streams *streams);

/**
 * Get the number of AVI stream with a certain media type.
 *
 * @param streams       The AVI streams.
 * @param media_type    The media type of the stream.
 *
 * @return              The number of AVI stream.
 */
PJ_DECL(unsigned)
pjmedia_avi_streams_get_num_streams_by_media(pjmedia_avi_streams *streams,
                                             pjmedia_type media_type);

/**
 * Return the idx-th stream of the AVI streams.
 *
 * @param streams       The AVI streams.
 * @param idx           The stream index.
 *
 * @return              The AVI stream or NULL if it does not exist.
 */
PJ_DECL(pjmedia_avi_stream *)
pjmedia_avi_streams_get_stream(pjmedia_avi_streams *streams,
                               unsigned idx);

/**
 * Return an AVI stream with a certain media type from the AVI streams.
 *
 * @param streams       The AVI streams.
 * @param start_idx     The starting index.
 * @param media_type    The media type of the stream.
 *
 * @return              The AVI stream or NULL if it does not exist.
 */
PJ_DECL(pjmedia_avi_stream *)
pjmedia_avi_streams_get_stream_by_media(pjmedia_avi_streams *streams,
                                        unsigned start_idx,
                                        pjmedia_type media_type);

/**
 * Return the media port of an AVI stream.
 *
 * @param stream        The AVI stream.
 *
 * @return              The media port.
 */
PJ_INLINE(pjmedia_port *)
pjmedia_avi_stream_get_port(pjmedia_avi_stream *stream)
{
    return (pjmedia_port *)stream;
}

/**
 * Get the data length, in bytes.
 *
 * @param stream        The AVI stream.
 *
 * @return              The length of the data, in bytes. Upon error it will
 *                      return negative value.
 */
PJ_DECL(pj_ssize_t) pjmedia_avi_stream_get_len(pjmedia_avi_stream *stream);


#if !DEPRECATED_FOR_TICKET_2251
/**
 * Register a callback to be called when the file reading has reached the
 * end of file. If the file is set to play repeatedly, then the callback
 * will be called multiple times. Note that only one callback can be 
 * registered for each AVI stream.
 *
 * @param stream        The AVI stream.
 * @param user_data     User data to be specified in the callback
 * @param cb            Callback to be called. If the callback returns non-
 *                      PJ_SUCCESS, the playback will stop. Note that if
 *                      application destroys the file port in the callback,
 *                      it must return non-PJ_SUCCESS here.
 *
 * @return              PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) 
pjmedia_avi_stream_set_eof_cb(pjmedia_avi_stream *stream,
                              void *user_data,
                              pj_status_t (*cb)(pjmedia_avi_stream *stream,
                                                void *usr_data));
#endif


/**
 * Register a callback to be called when the file reading has reached the
 * end of file. If the file is set to play repeatedly, then the callback
 * will be called multiple times. Note that only one callback can be 
 * registered for each AVI stream.
 *
 * @param stream        The AVI stream.
 * @param user_data     User data to be specified in the callback
 * @param cb            Callback to be called. Note that if
 *                      application wishes to stop the playback, it
 *                      can disconnect the port in the callback, and
 *                      only after all connections have been removed
 *                      could the application safely destroy the port.
 *
 * @return              PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) 
pjmedia_avi_stream_set_eof_cb2(pjmedia_avi_stream *stream,
                               void *user_data,
                               void (*cb)(pjmedia_avi_stream *stream,
                                          void *usr_data));


/**
 * @}
 */

/**
 * @defgroup PJMEDIA_AVI_FILE_WRITE AVI File Writer
 * @ingroup PJMEDIA_PORT
 * @brief Video and audio recording to AVI file
 * @{
 */

/**
 * Create avi streams to write to an AVI file. AVI writer supports
 * recording AVI file with uncompressed video format and
 * 16 bit PCM.
 *
 * Note that video recording file size can grow very quickly, and
 * once it reaches the maximum size specified, the file will be
 * automatically closed and the callback (if any) will be called.
 *
 * @param pool          Pool to create the streams.
 * @param filename      File name to write to.
 * @param max_fsize     Maximum file size.
 * @param num_streams   Number of streams to write. Typically this should be
 *                      2, one for video, and one for audio.
 * @param format        The format of the streams.
 * @param flags         Avi streams creation flags. Currently must be zero.
 * @param p_streams     Pointer to receive the avi streams instance.
 *
 * @return              PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t)
pjmedia_avi_writer_create_streams(pj_pool_t *pool,
                                  const char *filename,
                                  pj_uint32_t max_fsize,
                                  unsigned num_streams,
                                  const pjmedia_format format[],
                                  unsigned flags,
                                  pjmedia_avi_streams **p_streams);


/**
 * Register the callback to be called when the file writing has reached
 * maximum size.
 *
 * @param streams       The AVI writer streams.
 * @param user_data     User data to be specified in the callback, and will be
 *                      given on the callback.
 * @param cb            Callback to be called.
 *
 * @return              PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t)
pjmedia_avi_streams_set_cb(pjmedia_avi_streams *streams,
                           void *user_data,
                           void (*cb)(pjmedia_avi_streams *streams,
                                      void *usr_data));


PJ_END_DECL


#endif  /* __PJMEDIA_AVI_STREAM_H__ */
