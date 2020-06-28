#pragma once

#include "EventLogger.h"

static HMODULE s_hInst = nullptr;
static HMODULE s_hKernel32 = nullptr;
static CHAR s_szDllPath[MAX_PATH];
static TBLOG_PAYLOAD s_Payload;
static TBLOG_PAYLOAD s_ChildPayload;
static CRITICAL_SECTION s_csChildPayload;
static DWORD s_nTraceProcessId = 0;
static LONG s_nChildCnt = 0;
static PWCHAR s_pwEnvironment = nullptr;
static DWORD s_cwEnvironment = 0;
static PCHAR s_pbEnvironment = nullptr;
static DWORD s_cbEnvironment = 0;

static EventLogger s_eventLogger;

static CRITICAL_SECTION s_csPipe; // Guards access to hPipe.
static HANDLE s_hPipe = INVALID_HANDLE_VALUE;
static TBLOG_MESSAGE s_rMessage;

void VSafePrintf(PCSTR pszMsg, va_list args, PCHAR pszBuffer, LONG cbBuffer);
PCHAR SafePrintf(PCHAR pszBuffer, LONG cbBuffer, PCSTR pszMsg, ...);

LONG EnterFunc();
void ExitFunc();
void Print(PCSTR psz, ...);
void NoteRead(PCSTR psz);
void NoteRead(PCWSTR pwz);
void NoteWrite(PCSTR psz);
void NoteWrite(PCWSTR pwz);
void NoteDelete(PCSTR psz);
void NoteDelete(PCWSTR pwz);
void NoteCleanup(PCSTR psz);
void NoteCleanup(PCWSTR pwz);

PBYTE LoadFile(HANDLE hFile, DWORD cbFile);
static PCHAR RemoveReturns(PCHAR pszBuffer);
static PWCHAR RemoveReturns(PWCHAR pwzBuffer);

void AssertFailed(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine);

static void Copy(PWCHAR pwzDst, PCWSTR pwzSrc)
{
	while (*pwzSrc)
	{
		*pwzDst++ = *pwzSrc++;
	}

	*pwzDst = '\0';
}

static DWORD Size(PCWSTR pwzSrc)
{
	DWORD c = 0;
	while (pwzSrc[c])
	{
		c++;
	}

	return c;
}

static PCWSTR Save(PCWSTR pwzSrc)
{
	DWORD c = (Size(pwzSrc) + 1) * sizeof(WCHAR);
	PWCHAR pwzDst = (PWCHAR)GlobalAlloc(GPTR, c);
	CopyMemory(pwzDst, pwzSrc, c);

	return pwzDst;
}

static bool HasSpace(PCWSTR pwz)
{
	for (; *pwz; pwz++)
	{
		if (*pwz == ' ' || *pwz == '\t' || *pwz == '\r' || *pwz == '\n')
		{
			return true;
		}
	}

	return false;
}

static bool HasChar(PCWSTR pwz, WCHAR w)
{
	for (; *pwz; pwz++)
	{
		if (*pwz == w)
		{
			return true;
		}
	}

	return false;
}

static DWORD Compare(PCWSTR pwzA, PCWSTR pwzB)
{
	for (;;)
	{
		WCHAR cA = *pwzA++;
		WCHAR cB = *pwzB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

static DWORD Compare(PCWSTR pwzA, PCSTR pszB)
{
	for (;;)
	{
		WCHAR cA = *pwzA++;
		WCHAR cB = *pszB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

static DWORD Compare(PCSTR pszA, PCSTR pszB)
{
	for (;;)
	{
		CHAR cA = *pszA++;
		CHAR cB = *pszB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

///////////////////////////////////////////////////////////////////// VPrintf.
//
// Completely side-effect free printf replacement (but no FP numbers).
//
static PCHAR do_base(PCHAR pszOut, UINT64 nValue, UINT nBase, PCSTR pszDigits)
{
	CHAR szTmp[96];
	int nDigit = sizeof(szTmp)-2;
	for (; nDigit >= 0; nDigit--)
	{
		szTmp[nDigit] = pszDigits[nValue % nBase];
		nValue /= nBase;
	}

	for (nDigit = 0; nDigit < sizeof(szTmp) - 2 && szTmp[nDigit] == '0'; nDigit++)
	{
		// skip leading zeros.
	}

	for (; nDigit < sizeof(szTmp) - 1; nDigit++)
	{
		*pszOut++ = szTmp[nDigit];
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_str(PCHAR pszOut, PCHAR pszEnd, PCSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		*pszOut++ = *pszIn++;
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_wstr(PCHAR pszOut, PCHAR pszEnd, PCWSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		*pszOut++ = (CHAR)*pszIn++;
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_estr(PCHAR pszOut, PCHAR pszEnd, PCSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		if (*pszIn == '<')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'l';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '>')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'g';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '&')
		{
			if (pszOut + 5 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'm';
			*pszOut++ = 'p';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\"')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'q';
			*pszOut++ = 'u';
			*pszOut++ = 'o';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\'')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'p';
			*pszOut++ = 'o';
			*pszOut++ = 's';
			*pszOut++ = ';';
		}
		else if (*pszIn  < ' ')
		{
			BYTE c = (BYTE)(*pszIn++);
			if (c < 10 && pszOut + 4 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else if (c < 100 && pszOut + 5 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + ((c / 10) % 10);
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else if (c < 1000 && pszOut + 6 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + ((c / 100) % 10);
				*pszOut++ = '0' + ((c / 10) % 10);
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else
			{
				break;
			}
		}
		else
		{
			*pszOut++ = *pszIn++;
		}
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_ewstr(PCHAR pszOut, PCHAR pszEnd, PCWSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		if (*pszIn == '<')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'l';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '>')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'g';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '&')
		{
			if (pszOut + 5 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'm';
			*pszOut++ = 'p';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\"')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'q';
			*pszOut++ = 'u';
			*pszOut++ = 'o';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\'')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'p';
			*pszOut++ = 'o';
			*pszOut++ = 's';
			*pszOut++ = ';';
		}
		else if (*pszIn  < ' ' || *pszIn > 127)
		{
			WCHAR c = *pszIn++;
			if (c < 10 && pszOut + 4 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else if (c < 100 && pszOut + 5 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)((c / 10) % 10);
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else if (c < 1000 && pszOut + 6 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)((c / 100) % 10);
				*pszOut++ = '0' + (CHAR)((c / 10) % 10);
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else
			{
				break;
			}
		}
		else
		{
			*pszOut++ = (CHAR)*pszIn++;
		}
	}

	*pszOut = '\0';
	return pszOut;
}

#if _MSC_VER >= 1900
#pragma warning(push)
#pragma warning(disable:4456) // declaration hides previous local declaration
#endif

void VSafePrintf(PCSTR pszMsg, va_list args, PCHAR pszBuffer, LONG cbBuffer)
{
	PCHAR pszOut = pszBuffer;
	PCHAR pszEnd = pszBuffer + cbBuffer - 1;
	pszBuffer[0] = '\0';

	__try
	{
		while (*pszMsg && pszOut < pszEnd)
		{
			if (*pszMsg == '%')
			{
				CHAR szHead[4] = "";
				INT nLen;
				INT nWidth = 0;
				INT nPrecision = 0;
				bool fLeft = false;
				bool fPositive = false;
				bool fPound = false;
				bool fBlank = false;
				bool fZero = false;
				bool fDigit = false;
				bool fSmall = false;
				bool fLarge = false;
				bool f64Bit = false;
				PCSTR pszArg = pszMsg;

				pszMsg++;

				for (; (*pszMsg == '-' ||
						*pszMsg == '+' ||
						*pszMsg == '#' ||
						*pszMsg == ' ' ||
						*pszMsg == '0'); pszMsg++)
				{
					switch (*pszMsg)
					{
						case '-': fLeft = true; break;
						case '+': fPositive = true; break;
						case '#': fPound = true; break;
						case ' ': fBlank = true; break;
						case '0': fZero = true; break;
					}
				}

				if (*pszMsg == '*')
				{
					nWidth = va_arg(args, INT);
					pszMsg++;
				}
				else
				{
					while (*pszMsg >= '0' && *pszMsg <= '9')
					{
						nWidth = nWidth * 10 + (*pszMsg++ - '0');
					}
				}
				if (*pszMsg == '.')
				{
					pszMsg++;
					fDigit = true;
					if (*pszMsg == '*')
					{
						nPrecision = va_arg(args, INT);
						pszMsg++;
					}
					else
					{
						while (*pszMsg >= '0' && *pszMsg <= '9')
						{
							nPrecision = nPrecision * 10 + (*pszMsg++ - '0');
						}
					}
				}

				if (*pszMsg == 'h')
				{
					fSmall = true;
					pszMsg++;
				}
				else if (*pszMsg == 'l')
				{
					fLarge = true;
					pszMsg++;
				}
				else if (*pszMsg == 'I' && pszMsg[1] == '6' && pszMsg[2] == '4')
				{
					f64Bit = true;
					pszMsg += 3;
				}

				if (*pszMsg == 's' || *pszMsg == 'e' || *pszMsg == 'c')
				{
					// We ignore the length, precision, and alignment
					// to avoid using a temporary buffer.

					if (*pszMsg == 's')
					{
						// [GalenH] need to not use temp.
						PVOID pvData = va_arg(args, PVOID);

						pszMsg++;

						if (fSmall)
						{
							fLarge = false;
						}

						__try
						{
							if (pvData == nullptr)
							{
								pszOut = do_str(pszOut, pszEnd, "-nullptr-");
							}
							else if (fLarge)
							{
								pszOut = do_wstr(pszOut, pszEnd, (PWCHAR)pvData);
							}
							else
							{
								pszOut = do_str(pszOut, pszEnd, (PCHAR)pvData);
							}
						}
						__except(EXCEPTION_EXECUTE_HANDLER)
						{
							pszOut = do_str(pszOut, pszEnd, "-");
							pszOut = do_base(
								pszOut,
								(UINT64)pvData,
								16,
								"0123456789ABCDEF");
							pszOut = do_str(pszOut, pszEnd, "-");
						}
					}
					else if (*pszMsg == 'e')
					{
						// Escape the string.
						PVOID pvData = va_arg(args, PVOID);

						pszMsg++;

						if (fSmall)
						{
							fLarge = false;
						}

						__try
						{
							if (pvData == nullptr)
							{
								pszOut = do_str(pszOut, pszEnd, "-nullptr-");
							}
							else if (fLarge)
							{
								pszOut = do_ewstr(pszOut, pszEnd, (PWCHAR)pvData);
							}
							else
							{
								pszOut = do_estr(pszOut, pszEnd, (PCHAR)pvData);
							}
						}
						__except(EXCEPTION_EXECUTE_HANDLER)
						{
							pszOut = do_str(pszOut, pszEnd, "-");
							pszOut = do_base(
								pszOut,
								(UINT64)pvData,
								16,
								"0123456789ABCDEF");
							pszOut = do_str(pszOut, pszEnd, "-");
						}
					}
					else
					{
						CHAR szTemp[2];
						pszMsg++;

						szTemp[0] = (CHAR)va_arg(args, INT);
						szTemp[1] = '\0';
						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
				}
				else if (*pszMsg == 'd' || *pszMsg == 'i' || *pszMsg == 'o' ||
							*pszMsg == 'x' || *pszMsg == 'X' || *pszMsg == 'b' ||
							*pszMsg == 'u')
				{
					CHAR szTemp[128];
					UINT64 value;
					if (f64Bit)
					{
						value = va_arg(args, UINT64);
					}
					else
					{
						value = va_arg(args, UINT);
					}

					if (*pszMsg == 'x')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 16, "0123456789abcdef") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}
					else if (*pszMsg == 'X')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 16, "0123456789ABCDEF") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0X");
						}
					}
					else if (*pszMsg == 'd')
					{
						pszMsg++;
						if ((INT64)value < 0)
						{
							value = -(INT64)value;
							do_str(szHead, szHead + sizeof(szHead) - 1, "-");
						}
						else if (fPositive)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, "+");
							}
						}
						else if (fBlank)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, " ");
							}
						}
						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}
					else if (*pszMsg == 'u')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}
					else if (*pszMsg == 'o')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 8, "01234567") - szTemp);
						nPrecision = 0;

						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0");
						}
					}
					else if (*pszMsg == 'b')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 2, "01") - szTemp);
						nPrecision = 0;

						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0b");
						}
					}
					else
					{
						pszMsg++;
						if ((INT64)value < 0)
						{
							value = -(INT64)value;
							do_str(szHead, szHead + sizeof(szHead) - 1, "-");
						}
						else if (fPositive)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, "+");
							}
						}
						else if (fBlank)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, " ");
							}
						}

						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}

					INT nHead = 0;
					for (; szHead[nHead]; nHead++)
					{
						// Count characters in head string.
					}

					if (fLeft)
					{
						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
							nLen += nHead;
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = ' ';
						}
					}
					else if (fZero)
					{
						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
							nLen += nHead;
						}

						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = '0';
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
					else
					{
						if (nHead)
						{
							nLen += nHead;
						}

						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = ' ';
						}

						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
				}
				else if (*pszMsg == 'p')
				{
					CHAR szTemp[64];
					ULONG_PTR value;
					value = va_arg(args, ULONG_PTR);

					if (*pszMsg == 'p')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, (UINT64)value, 16, "0123456789abcdef") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}
					else
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, (UINT64)value, 16, "0123456789ABCDEF") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}

					INT nHead = 0;
					for (; szHead[nHead]; nHead++)
					{
						// Count characters in head string.
					}

					if (nHead)
					{
						pszOut = do_str(pszOut, pszEnd, szHead);
						nLen += nHead;
					}

					for (; nLen < nWidth && pszOut < pszEnd; nLen++)
					{
						*pszOut++ = '0';
					}

					pszOut = do_str(pszOut, pszEnd, szTemp);
				}
				else
				{
					pszMsg++;
					while (pszArg < pszMsg && pszOut < pszEnd)
					{
						*pszOut++ = *pszArg++;
					}
				}
			}
			else
			{
				if (pszOut < pszEnd)
				{
					*pszOut++ = *pszMsg++;
				}
			}
		}

		*pszOut = '\0';
		pszBuffer[cbBuffer - 1] = '\0';
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		PCHAR pszOut = pszBuffer;
		*pszOut = '\0';
		pszOut = do_str(pszOut, pszEnd, "-exception:");
		pszOut = do_base(pszOut, (UINT64)GetExceptionCode(), 10, "0123456789");
		pszOut = do_str(pszOut, pszEnd, "-");
	}
}

#if _MSC_VER >= 1900
#pragma warning(pop)
#endif

PCHAR SafePrintf(PCHAR pszBuffer, LONG cbBuffer, PCSTR pszMsg, ...)
{
	va_list args;
	va_start(args, pszMsg);
	VSafePrintf(pszMsg, args, pszBuffer, cbBuffer);
	va_end(args);

	while (*pszBuffer)
	{
		pszBuffer++;
	}

	return pszBuffer;
}

/////////////////////////////////////////////////////////////
// Detours
//

bool CreateProcessInternals(
	HANDLE hProcess,
	PCHAR pszId)
{
	EnterCriticalSection(&s_csChildPayload);

	ZeroMemory(&s_ChildPayload, sizeof(s_ChildPayload));
	CopyMemory(&s_ChildPayload, &s_Payload, sizeof(s_ChildPayload));

	s_ChildPayload.nParentProcessId = GetCurrentProcessId();
	s_ChildPayload.rGeneology[s_ChildPayload.nGeneology]
		= (DWORD)InterlockedIncrement(&s_nChildCnt);
	s_ChildPayload.nGeneology++;

	DetourCopyPayloadToProcess(hProcess, s_guidTrace, &s_ChildPayload, sizeof(s_ChildPayload));

	for (DWORD i = 0; i < s_ChildPayload.nGeneology; i++)
	{
		pszId = SafePrintf(pszId, 16, "%d.", s_ChildPayload.rGeneology[i]);
	}

	*pszId = '\0';

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}