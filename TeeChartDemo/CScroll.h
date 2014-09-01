// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Windows\\System32\\TeeChart8.ocx" no_namespace
// CScroll wrapper class

class CScroll : public COleDispatchDriver
{
public:
	CScroll(){} // Calls COleDispatchDriver default constructor
	CScroll(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CScroll(const CScroll& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IScroll methods
public:
	long get_Enable()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Enable(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MouseButton()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MouseButton(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IScroll properties
public:

};
