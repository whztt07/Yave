
/*******************************
Copyright (c) 2016-2018 Gr�goire Angerand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**********************************/
#ifndef YAVE_ASSETS_FOLDERASSETSTORE_H
#define YAVE_ASSETS_FOLDERASSETSTORE_H

#include <y/serde/serde.h>

#include "AssetStore.h"

#include <unordered_map>
#include <map>
#include <mutex>


#ifndef YAVE_NO_STDFS

namespace yave {

class FolderAssetStore final : public AssetStore {

	struct Entry {
		core::String name;
		AssetId id;

		y_serde(id, name)
	};

	// same as LocalFileSystemModel but rooted in a folder
	class FolderFileSystemModel final : public LocalFileSystemModel {
		public:
			FolderFileSystemModel(std::string_view root);
			const core::String& root_path() const;

			core::String current_path() const override;
			bool exists(std::string_view path) const override;
			bool is_directory(std::string_view path) const override;
			core::String absolute(std::string_view path) const override;
			void for_each(std::string_view path, const for_each_f& func) const override;
			bool create_directory(std::string_view path) const override;
			bool remove(std::string_view path) const override;
			bool rename(std::string_view from, std::string_view to) const override;

		private:
			core::String _root;
	};

	public:
		FolderAssetStore(std::string_view path = "./store");
		~FolderAssetStore() override;

		const FileSystemModel* filesystem() const override;

		AssetId import(io::ReaderRef data, std::string_view dst_name) override;

		AssetId id(std::string_view name) const override;
		io::ReaderRef data(AssetId id) const override;

		void remove(AssetId id) override;
		void rename(AssetId id, std::string_view new_name) override;
		void remove(std::string_view name) override;
		void rename(std::string_view from, std::string_view to) override;

		void clean_index();

	private:
		void write_index() const;
		void read_index();

		FolderFileSystemModel _filesystem;
		core::String _index_file_path;

		mutable std::recursive_mutex _lock;

		// TODO optimize ?
		std::unordered_map<AssetId, Entry*> _from_id;
		std::unordered_map<core::String, std::unique_ptr<Entry>> _from_name;

		AssetIdFactory _id_factory;
};

}

#endif // YAVE_NO_STDFS

#endif // YAVE_ASSETS_FOLDERASSETSTORE_H
