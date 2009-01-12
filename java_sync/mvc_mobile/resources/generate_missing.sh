#!/bin/sh
names=`find ../*src -name '*.*' |
	grep -v /.svn/ | 
	xargs grep --no-filename -o '${[^}]\+}' | 
	cut -c3- | 
	tr -d '}' |
	sort | uniq`
for res in messages*.properties;do
	tmp=`mktemp .$res.XXXXXX`
	for name in $names; do
		value=`grep ^$name= $res | sort | uniq | cut -d= -f2`
		if [ -z "$value" ];then
			value="?$name?"
		fi
		echo $name=$value >> $tmp
	done


	echo '# Copyright 2007 Krzysztof Rzymkowski <rzymek@users.sourceforge.net>' > $res
	echo '# ' >> $res
	echo '# Licensed under the Apache License, Version 2.0 (the "License");' >> $res
	echo '# you may not use this file except in compliance with the License.' >> $res
	echo '# You may obtain a copy of the License at' >> $res
	echo '# ' >> $res
	echo '#     http://www.apache.org/licenses/LICENSE-2.0' >> $res
	echo '# ' >> $res
	echo '# Unless required by applicable law or agreed to in writing, software' >> $res
	echo '# distributed under the License is distributed on an "AS IS" BASIS,' >> $res
	echo '# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.' >> $res
	echo '# See the License for the specific language governing permissions and' >> $res
	echo '# limitations under the License.' >> $res

	sort < $tmp >> $res
	rm -f $tmp
done

