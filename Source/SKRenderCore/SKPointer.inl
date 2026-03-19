template <class T>
SKPointer<T>::SKPointer (T* pObject)
{
	m_pObject = pObject;
	if (m_pObject)
	{
		m_pObject->IncreRef();
	}
}
//----------------------------------------------------------------------------
template <class T>
SKPointer<T>::SKPointer (const SKPointer& rPointer)
{
	m_pObject = rPointer.m_pObject;
	if (m_pObject)
	{
		m_pObject->IncreRef();
		
	}
}
//----------------------------------------------------------------------------
template <class T>
SKPointer<T>::~SKPointer ()
{
	if (m_pObject)
	{
		m_pObject->DecreRef();
		m_pObject = NULL;
	}
}
//----------------------------------------------------------------------------

template <class T>
T& SKPointer<T>::operator* () const
{
	return *m_pObject;
}
//----------------------------------------------------------------------------
template <class T>
T* SKPointer<T>::operator-> () const
{
	return m_pObject;
}
//----------------------------------------------------------------------------
template <class T>
SKPointer<T>& SKPointer<T>::operator= (T* pObject)
{
	if (m_pObject != pObject)
	{
		if (pObject)
		{
			pObject->IncreRef();
		}

		if (m_pObject)
		{
			m_pObject->DecreRef();
		}

		m_pObject = pObject;
	}
	return *this;
}
//----------------------------------------------------------------------------
template <class T>
SKPointer<T>& SKPointer<T>::operator= (const SKPointer& rPointer)
{
	if (m_pObject != rPointer.m_pObject)
	{
		if (rPointer.m_pObject)
		{
			rPointer.m_pObject->IncreRef();
		}

		if (m_pObject)
		{
			m_pObject->DecreRef();
		}

		m_pObject = rPointer.m_pObject;
	}
	return *this;
}
//----------------------------------------------------------------------------
template <class T>
bool SKPointer<T>::operator== (T* pObject) const
{
	return (m_pObject == pObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SKPointer<T>::operator!= (T* pObject) const
{
	return (m_pObject != pObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SKPointer<T>::operator== (const SKPointer& rPointer) const
{
	return (m_pObject == rPointer.m_pObject);
}
//----------------------------------------------------------------------------
template <class T>
bool SKPointer<T>::operator!= (const SKPointer& rPointer) const
{
	return (m_pObject != rPointer.m_pObject);
}
//----------------------------------------------------------------------------
template <class T>
SKPointer<T>::operator T *()const 
{
	return m_pObject;
}