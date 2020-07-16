#ifndef PROCESSORDATA_H
#define PROCESSORDATA_H

#include "Enobio3GAPI_global.h"

/*!
 * \class PData PData.h
 *
 * \brief Generic data class that contains an unspecific data object.
 */
class PData
{
public:
    /*!
     * Empty constructor
     */
    PData () { m_data = NULL; }

    /*!
     * Initialization constructor
     *
     * \param data Pointer to the data to be stored
     */
    PData (void * data) { setData(data); }

    /*!
     * Virtual destructor
     */
    virtual ~PData () { }

    /*!
     * It returns a pointer to the stored data.
     *
     * \return Generic pointer to the stored data.
     */
    void * getData () const { return m_data; }

    /*!
     * It holds the provided data.
     *
     * \param data Pointer to the data to be held
     */
    void setData (void * data) { m_data = data; }

private:
    /*!
     * Unspecific pointer to the Data stored by the class.
     */
    void * m_data;
};

#endif // PROCESSORDATA_H
