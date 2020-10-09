#include "helper_p.h"
#include <QDebug>

int helper::check(sp_return result)
{
    /* For this example we'll just exit on any error by calling abort(). */
    char* error_message;

    switch (result) {
    case SP_ERR_ARG:
        error_message = sp_last_error_message();
        qDebug("Error: Invalid argument: %s", error_message);
        sp_free_error_message(error_message);
        abort();
    case SP_ERR_FAIL:
        error_message = sp_last_error_message();
        qDebug("Error: Failed: %s", error_message);
        sp_free_error_message(error_message);
        abort();
    case SP_ERR_SUPP:
        qDebug("Error: Not supported.");
        abort();
    case SP_ERR_MEM:
        qDebug("Error: Couldn't allocate memory.");
        abort();
    case SP_OK:
    default:
        return result;
    }
}
