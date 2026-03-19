
#ifndef SKMONITOR_H
#define SKMONITOR_H
#include "SKGraphic.h"
#include "SKSynchronize.h"
#include "SKThread.h"
#include "SKString.h"
#include "SKArray.h"
#include "SKResource.h"
#include "SKSafeQueue.h"
#ifdef WINDOWS
namespace SKEngine2
{
	struct FileOperation
	{
		enum Type
		{
			Added,
			Removed, 
			Modified
		};
	};
	struct FileChangedArgs
	{
		FileOperation::Type	Operation;
		SKString			FilePath;
		bool operator==(const FileChangedArgs& rhs) const;
	};
	FORCEINLINE bool FileChangedArgs::operator==(const FileChangedArgs& rhs) const
	{
		return (this->Operation == rhs.Operation) &&
			(this->FilePath == rhs.FilePath);
	}
	class SKGRAPHIC_API SKResourceChangedHandler
	{
	public:
		SKResourceChangedHandler()
		{

		}
		virtual ~SKResourceChangedHandler() {}
		// ļǷԴ
		bool CheckFileType(const SKString & path)
		{
			return path.GetSubStringIndex(SKResource::GetFileSuffix(GetResourceType()), 1) != -1;
		}
		// ļ¼
		virtual void OnFileChanged(const FileChangedArgs& args) = 0;

		virtual unsigned int GetResourceType() const = 0;
	};
	class SKGRAPHIC_API SKAnimChangedHandler : public SKResourceChangedHandler
	{
			
	public:
		SKAnimChangedHandler()
		{

		}
		virtual ~SKAnimChangedHandler()
		{

		}
		virtual unsigned int GetResourceType() const
		{
			return SKResource::RT_ACTION;
		}
		/// ļ¼
		virtual void OnFileChanged(const FileChangedArgs& args)
		{

		}
	};
	class SKMonitor; 
	class SKGRAPHIC_API SKFileMonitor : public SKThread
	{
	public:
		SKFileMonitor(SKMonitor * pOwner);
		virtual ~SKFileMonitor();

		// üĿ¼
		void SetDirectory(const SKString& s);
		// ȡüĿ¼
		const SKString & GetDirectory() const;

		// Ƿʹ·
		void SetUseRelativePath(bool b);
		// Ƿʹ·
		bool IsUseRelativePath() const;

	private:
		virtual void Run();
		virtual const TCHAR* GetThreadName();

		void NotifyHandler();
	private:
		static const size_t BufferSize = 4096;
		SKString m_directory;
		bool m_isUseRelativePath;
		unsigned char* m_pReadBuffer;
		SKMonitor * m_pOwner;
	};

	//------------------------------------------------------------------------------
	FORCEINLINE const SKString& SKFileMonitor::GetDirectory() const
	{
		return m_directory;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE void SKFileMonitor::SetDirectory(const SKString& s)
	{

		m_directory = s;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE void SKFileMonitor::SetUseRelativePath(bool b)
	{
		m_isUseRelativePath = b;
	}

	//------------------------------------------------------------------------------
	FORCEINLINE bool SKFileMonitor::IsUseRelativePath() const
	{
		return m_isUseRelativePath;
	}
	class SKGRAPHIC_API SKMonitor 
	{
	public:
		SKMonitor()
		{

		}
		virtual ~SKMonitor()
		{

		}
	protected:
		friend class SKFileMonitor;
		virtual void OnFileChanged(const FileChangedArgs& args) = 0;
	};
	class SKGRAPHIC_API SKResourceMonitor : public SKMonitor
	{
	public:
		SKResourceMonitor();
		virtual ~SKResourceMonitor();

		/// ʼ
		bool Open();
		/// 
		void Update(double dAppTime);
		/// 
		void Close();

		/// üĿ¼
		void SetMonitorDirectory(const SKString& path);

		/// עļʹӿ
		void RegisterHandler(SKResourceChangedHandler* handler);
		/// ɾӦӿ
		void UnregisterHandler(SKResourceChangedHandler* handler);
		static SKResourceMonitor * ms_pResourceMonitor;
	protected:
		void OnFileChanged(const FileChangedArgs& args);

	private:
		SKFileMonitor* m_fileMonitor;
		SKSafeQueue<FileChangedArgs> m_changedFiles;
		SKArray<SKResourceChangedHandler*> m_handlers;///< in main thread
	};
};
#endif
#endif
