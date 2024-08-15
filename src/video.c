#include "video.h"

#include <wp/wp.h>

#include "device.h"
#include "endpoint.h"
#include "wp.h"

struct _AstalWpVideo {
    GObject parent_instance;
};

typedef struct {
    AstalWpWp *wp;
} AstalWpVideoPrivate;

G_DEFINE_FINAL_TYPE_WITH_PRIVATE(AstalWpVideo, astal_wp_video, G_TYPE_OBJECT);

typedef enum {
    ASTAL_WP_VIDEO_SIGNAL_SOURCE_ADDED,
    ASTAL_WP_VIDEO_SIGNAL_SOURCE_REMOVED,
    ASTAL_WP_VIDEO_SIGNAL_SINK_ADDED,
    ASTAL_WP_VIDEO_SIGNAL_SINK_REMOVED,
    ASTAL_WP_VIDEO_SIGNAL_STREAM_ADDED,
    ASTAL_WP_VIDEO_SIGNAL_STREAM_REMOVED,
    ASTAL_WP_VIDEO_SIGNAL_RECORDER_ADDED,
    ASTAL_WP_VIDEO_SIGNAL_RECORDER_REMOVED,
    ASTAL_WP_VIDEO_SIGNAL_DEVICE_ADDED,
    ASTAL_WP_VIDEO_SIGNAL_DEVICE_REMOVED,
    ASTAL_WP_VIDEO_N_SIGNALS
} AstalWpWpSignals;

static guint astal_wp_video_signals[ASTAL_WP_VIDEO_N_SIGNALS] = {
    0,
};

typedef enum {
    ASTAL_WP_VIDEO_PROP_SOURCE = 1,
    ASTAL_WP_VIDEO_PROP_SINK,
    ASTAL_WP_VIDEO_PROP_STREAMS,
    ASTAL_WP_VIDEO_PROP_RECORDERS,
    ASTAL_WP_VIDEO_PROP_DEVICES,
    ASTAL_WP_VIDEO_N_PROPERTIES,
} AstalWpVideoProperties;

static GParamSpec *astal_wp_video_properties[ASTAL_WP_VIDEO_N_PROPERTIES] = {
    NULL,
};

/**
 * astal_wp_video_get_source:
 * @self: the AstalWpVideo object
 * @id: the id of the endpoint
 *
 * Returns: (transfer none) (nullable): the source with the given id
 */
AstalWpEndpoint *astal_wp_video_get_speaker(AstalWpVideo *self, guint id) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    AstalWpEndpoint *endpoint = astal_wp_wp_get_endpoint(priv->wp, id);
    if (astal_wp_endpoint_get_media_class(endpoint) == ASTAL_WP_MEDIA_CLASS_VIDEO_SOURCE)
        return endpoint;
    return NULL;
}

/**
 * astal_wp_video_get_sink:
 * @self: the AstalWpVideo object
 * @id: the id of the endpoint
 *
 * Returns: (transfer none) (nullable): the sink with the given id
 */
AstalWpEndpoint *astal_wp_video_get_sink(AstalWpVideo *self, guint id) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    AstalWpEndpoint *endpoint = astal_wp_wp_get_endpoint(priv->wp, id);
    if (astal_wp_endpoint_get_media_class(endpoint) == ASTAL_WP_MEDIA_CLASS_VIDEO_SINK)
        return endpoint;
    return NULL;
}

/**
 * astal_wp_video_get_stream:
 * @self: the AstalWpVideo object
 * @id: the id of the endpoint
 *
 * Returns: (transfer none) (nullable): the stream with the given id
 */
AstalWpEndpoint *astal_wp_video_get_stream(AstalWpVideo *self, guint id) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    AstalWpEndpoint *endpoint = astal_wp_wp_get_endpoint(priv->wp, id);
    if (astal_wp_endpoint_get_media_class(endpoint) == ASTAL_WP_MEDIA_CLASS_VIDEO_STREAM)
        return endpoint;
    return NULL;
}

/**
 * astal_wp_video_get_recorder:
 * @self: the AstalWpVideo object
 * @id: the id of the endpoint
 *
 * Returns: (transfer none) (nullable): the recorder with the given id
 */
AstalWpEndpoint *astal_wp_video_get_recorder(AstalWpVideo *self, guint id) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    AstalWpEndpoint *endpoint = astal_wp_wp_get_endpoint(priv->wp, id);
    if (astal_wp_endpoint_get_media_class(endpoint) == ASTAL_WP_MEDIA_CLASS_VIDEO_RECORDER)
        return endpoint;
    return NULL;
}

/**
 * astal_wp_video_get_device:
 * @self: the AstalWpVideo object
 * @id: the id of the device
 *
 * Returns: (transfer none) (nullable): the device with the given id
 */
AstalWpDevice *astal_wp_video_get_device(AstalWpVideo *self, guint id) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    AstalWpDevice *device = astal_wp_wp_get_device(priv->wp, id);
    if (astal_wp_device_get_device_type(device) == ASTAL_WP_DEVICE_TYPE_VIDEO) return device;
    return NULL;
}

/**
 * astal_wp_video_get_sources:
 * @self: the AstalWpVideo object
 *
 * Returns: (transfer container) (nullable) (type GList(AstalWpEndpoint)): a GList containing the
 * video sources
 */
GList *astal_wp_video_get_sources(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);
    GList *eps = astal_wp_wp_get_endpoints(priv->wp);
    GList *list = NULL;

    for (GList *l = eps; l != NULL; l = l->next) {
        if (astal_wp_endpoint_get_media_class(l->data) == ASTAL_WP_MEDIA_CLASS_VIDEO_SOURCE) {
            list = g_list_append(list, l->data);
        }
    }
    g_list_free(eps);
    return list;
}

/**
 * astal_wp_video_get_sinks
 * @self: the AstalWpVideo object
 *
 * Returns: (transfer container) (nullable) (type GList(AstalWpEndpoint)): a GList containing the
 * video sinks
 */
GList *astal_wp_video_get_sinks(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);
    GList *eps = astal_wp_wp_get_endpoints(priv->wp);
    GList *list = NULL;

    for (GList *l = eps; l != NULL; l = l->next) {
        if (astal_wp_endpoint_get_media_class(l->data) == ASTAL_WP_MEDIA_CLASS_VIDEO_SINK) {
            list = g_list_append(list, l->data);
        }
    }
    g_list_free(eps);
    return list;
}

/**
 * astal_wp_video_get_recorders:
 * @self: the AstalWpVideo object
 *
 * Returns: (transfer container) (nullable) (type GList(AstalWpEndpoint)): a GList containing the
 * video recorders
 */
GList *astal_wp_video_get_recorders(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);
    GList *eps = astal_wp_wp_get_endpoints(priv->wp);
    GList *list = NULL;

    for (GList *l = eps; l != NULL; l = l->next) {
        if (astal_wp_endpoint_get_media_class(l->data) == ASTAL_WP_MEDIA_CLASS_VIDEO_RECORDER) {
            list = g_list_append(list, l->data);
        }
    }
    g_list_free(eps);
    return list;
}

/**
 * astal_wp_video_get_streams:
 * @self: the AstalWpVideo object
 *
 * Returns: (transfer container) (nullable) (type GList(AstalWpEndpoint)): a GList containing the
 * video streams
 */
GList *astal_wp_video_get_streams(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);
    GList *eps = astal_wp_wp_get_endpoints(priv->wp);
    GList *list = NULL;

    for (GList *l = eps; l != NULL; l = l->next) {
        if (astal_wp_endpoint_get_media_class(l->data) == ASTAL_WP_MEDIA_CLASS_VIDEO_STREAM) {
            list = g_list_append(list, l->data);
        }
    }
    g_list_free(eps);
    return list;
}

/**
 * astal_wp_video_get_devices:
 * @self: the AstalWpAudio object
 *
 * Returns: (transfer container) (nullable) (type GList(AstalWpVideo)): a GList containing the
 * devices
 */
GList *astal_wp_video_get_devices(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);
    GList *eps = astal_wp_wp_get_devices(priv->wp);
    GList *list = NULL;

    for (GList *l = eps; l != NULL; l = l->next) {
        if (astal_wp_device_get_device_type(l->data) == ASTAL_WP_DEVICE_TYPE_VIDEO) {
            list = g_list_append(list, l->data);
        }
    }
    g_list_free(eps);
    return list;
}

static void astal_wp_video_get_property(GObject *object, guint property_id, GValue *value,
                                        GParamSpec *pspec) {
    AstalWpVideo *self = ASTAL_WP_VIDEO(object);

    switch (property_id) {
        case ASTAL_WP_VIDEO_PROP_SOURCE:
            g_value_set_pointer(value, astal_wp_video_get_sources(self));
            break;
        case ASTAL_WP_VIDEO_PROP_SINK:
            g_value_set_pointer(value, astal_wp_video_get_sinks(self));
            break;
        case ASTAL_WP_VIDEO_PROP_RECORDERS:
            g_value_set_pointer(value, astal_wp_video_get_recorders(self));
            break;
        case ASTAL_WP_VIDEO_PROP_STREAMS:
            g_value_set_pointer(value, astal_wp_video_get_streams(self));
            break;
        case ASTAL_WP_VIDEO_PROP_DEVICES:
            g_value_set_pointer(value, astal_wp_video_get_devices(self));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

void astal_wp_video_device_added(AstalWpVideo *self, gpointer object) {
    AstalWpDevice *device = ASTAL_WP_DEVICE(object);
    if (astal_wp_device_get_device_type(device) == ASTAL_WP_DEVICE_TYPE_VIDEO) {
        g_signal_emit_by_name(self, "device-added", device);
        g_object_notify(G_OBJECT(self), "devices");
    }
}

static void astal_wp_video_device_removed(AstalWpVideo *self, gpointer object) {
    AstalWpDevice *device = ASTAL_WP_DEVICE(object);
    if (astal_wp_device_get_device_type(device) == ASTAL_WP_DEVICE_TYPE_VIDEO) {
        g_signal_emit_by_name(self, "device-removed", device);
        g_object_notify(G_OBJECT(self), "devices");
    }
}

static void astal_wp_video_object_added(AstalWpVideo *self, gpointer object) {
    AstalWpEndpoint *endpoint = ASTAL_WP_ENDPOINT(object);
    switch (astal_wp_endpoint_get_media_class(endpoint)) {
        case ASTAL_WP_MEDIA_CLASS_VIDEO_SOURCE:
            g_signal_emit_by_name(self, "source-added", endpoint);
            g_object_notify(G_OBJECT(self), "sources");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_SINK:
            g_signal_emit_by_name(self, "sink-added", endpoint);
            g_object_notify(G_OBJECT(self), "sinks");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_STREAM:
            g_signal_emit_by_name(self, "stream-added", endpoint);
            g_object_notify(G_OBJECT(self), "streams");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_RECORDER:
            g_signal_emit_by_name(self, "recorder-added", endpoint);
            g_object_notify(G_OBJECT(self), "recorders");
            break;
        default:
            break;
    }
}

static void astal_wp_video_object_removed(AstalWpAudio *self, gpointer object) {
    AstalWpEndpoint *endpoint = ASTAL_WP_ENDPOINT(object);
    switch (astal_wp_endpoint_get_media_class(endpoint)) {
        case ASTAL_WP_MEDIA_CLASS_VIDEO_SOURCE:
            g_signal_emit_by_name(self, "source-removed", endpoint);
            g_object_notify(G_OBJECT(self), "sources");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_SINK:
            g_signal_emit_by_name(self, "sink-removed", endpoint);
            g_object_notify(G_OBJECT(self), "sinks");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_STREAM:
            g_signal_emit_by_name(self, "stream-removed", endpoint);
            g_object_notify(G_OBJECT(self), "streams");
            break;
        case ASTAL_WP_MEDIA_CLASS_VIDEO_RECORDER:
            g_signal_emit_by_name(self, "recorder-removed", endpoint);
            g_object_notify(G_OBJECT(self), "recorders");
            break;
        default:
            break;
    }
}

/**
 * astal_wp_video_get_default
 *
 * Returns: (nullable) (transfer none): gets the default video object.
 */
AstalWpVideo *astal_wp_video_get_default() {
    static AstalWpVideo *self = NULL;

    if (self == NULL) self = g_object_new(ASTAL_WP_TYPE_VIDEO, NULL);

    return self;
}

/**
 * astal_wp_get_default_video
 *
 * Returns: (nullable) (transfer none): gets the default video object.
 */
AstalWpVideo *astal_wp_get_default_video() { return astal_wp_video_get_default(); }

static void astal_wp_video_init(AstalWpVideo *self) {
    AstalWpVideoPrivate *priv = astal_wp_video_get_instance_private(self);

    priv->wp = astal_wp_wp_get_default();

    g_signal_connect_swapped(priv->wp, "endpoint-added", G_CALLBACK(astal_wp_video_object_added),
                             self);
    g_signal_connect_swapped(priv->wp, "endpoint-removed",
                             G_CALLBACK(astal_wp_video_object_removed), self);
    g_signal_connect_swapped(priv->wp, "device-added", G_CALLBACK(astal_wp_video_device_added),
                             self);
    g_signal_connect_swapped(priv->wp, "device-removed", G_CALLBACK(astal_wp_video_device_removed),
                             self);
}

static void astal_wp_video_class_init(AstalWpVideoClass *class) {
    GObjectClass *object_class = G_OBJECT_CLASS(class);
    object_class->get_property = astal_wp_video_get_property;

    /**
     * AstalWpVideo:sources: (type GList(AstalWpEndpoint)) (transfer container)
     *
     * A list of AstalWpEndpoint objects
     */
    astal_wp_video_properties[ASTAL_WP_VIDEO_PROP_SOURCE] =
        g_param_spec_pointer("sources", "sources", "sources", G_PARAM_READABLE);

    /**
     * AstalWpVideo:sinks: (type GList(AstalWpEndpoint)) (transfer container)
     *
     * A list of AstalWpEndpoint objects
     */
    astal_wp_video_properties[ASTAL_WP_VIDEO_PROP_SINK] =
        g_param_spec_pointer("sinks", "sinks", "sinks", G_PARAM_READABLE);

    /**
     * AstalWpVideo:recorder: (type GList(AstalWpEndpoint)) (transfer container)
     *
     * A list of AstalWpEndpoint objects
     */
    astal_wp_video_properties[ASTAL_WP_VIDEO_PROP_RECORDERS] =
        g_param_spec_pointer("recorders", "recorders", "recorders", G_PARAM_READABLE);

    /**
     * AstalWpVideo:streams: (type GList(AstalWpEndpoint)) (transfer container)
     *
     * A list of AstalWpEndpoint objects
     */
    astal_wp_video_properties[ASTAL_WP_VIDEO_PROP_STREAMS] =
        g_param_spec_pointer("streams", "streams", "streams", G_PARAM_READABLE);

    /**
     * AstalWpVideo:devices: (type GList(AstalWpEndpoint)) (transfer container)
     *
     * A list of AstalWpEndpoint objects
     */
    astal_wp_video_properties[ASTAL_WP_VIDEO_PROP_DEVICES] =
        g_param_spec_pointer("devices", "devices", "devices", G_PARAM_READABLE);

    g_object_class_install_properties(object_class, ASTAL_WP_VIDEO_N_PROPERTIES,
                                      astal_wp_video_properties);

    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_SOURCE_ADDED] =
        g_signal_new("source-added", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_SOURCE_REMOVED] =
        g_signal_new("source-removed", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_SINK_ADDED] =
        g_signal_new("sink-added", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_SINK_REMOVED] =
        g_signal_new("sink-removed", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_STREAM_ADDED] =
        g_signal_new("stream-added", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_SOURCE_REMOVED] =
        g_signal_new("stream-removed", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_RECORDER_ADDED] =
        g_signal_new("recorder-added", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_RECORDER_REMOVED] =
        g_signal_new("recorder-removed", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL,
                     NULL, NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_ENDPOINT);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_DEVICE_ADDED] =
        g_signal_new("device-added", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_DEVICE);
    astal_wp_video_signals[ASTAL_WP_VIDEO_SIGNAL_DEVICE_REMOVED] =
        g_signal_new("device-removed", G_TYPE_FROM_CLASS(class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                     NULL, G_TYPE_NONE, 1, ASTAL_WP_TYPE_DEVICE);
}
