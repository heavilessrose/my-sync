/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.jobs;

import java.io.IOException;

import com.astrientlabs.log.Logger;
import com.astrientlabs.search.SearchQuery;
import com.astrientlabs.ui.Window;

public class SearchJob implements Runnable
{
    protected Window window;
    protected SearchQuery searchQuery;
    
    public SearchJob(Window window, SearchQuery searchQuery)
    {
        this.window = window;
        this.searchQuery = searchQuery;
    }

    public void run()
    {
        try
        {
            //Logger.instance.log("imageService","Fetching results: " + searchQuery.getTerm());
            searchQuery.execute();
            //Logger.instance.log("imageService",searchQuery.getTerm() + ": results: " + searchQuery.getResults().size());
        }
        catch (IOException t)
        {
            Logger.instance.log("searchJob.run().2",t);
            
            window.setError(t);
        }
    }
}
