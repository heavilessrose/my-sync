/**
 * Copyright (C) 2007 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

package com.example.android.apis.app;

import android.content.Intent;
import android.content.res.TypedArray;
import android.net.Uri;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceScreen;

import com.example.android.apis.R;

public class PreferencesFromCode extends PreferenceActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setPreferenceScreen(createPreferenceHierarchy());
    }

    private PreferenceScreen createPreferenceHierarchy() {
        // Root
        PreferenceScreen root = getPreferenceManager().createPreferenceScreen(this);
        
        // Inline preferences 
        PreferenceCategory inlinePrefCat = new PreferenceCategory(this);
        inlinePrefCat.setTitle(R.string.inline_preferences);
        root.addPreference(inlinePrefCat);
        
        // Toggle preference
        CheckBoxPreference togglePref = new CheckBoxPreference(this);
        togglePref.setKey("toggle_preference");
        togglePref.setTitle(R.string.title_toggle_preference);
        togglePref.setSummary(R.string.summary_toggle_preference);
        inlinePrefCat.addPreference(togglePref);
                
        // Dialog based preferences
        PreferenceCategory dialogBasedPrefCat = new PreferenceCategory(this);
        dialogBasedPrefCat.setTitle(R.string.dialog_based_preferences);
        root.addPreference(dialogBasedPrefCat);

        // Edit text preference
        EditTextPreference editTextPref = new EditTextPreference(this);
        editTextPref.setDialogTitle(R.string.dialog_title_edittext_preference);
        editTextPref.setKey("edittext_preference");
        editTextPref.setTitle(R.string.title_edittext_preference);
        editTextPref.setSummary(R.string.summary_edittext_preference);
        dialogBasedPrefCat.addPreference(editTextPref);
        
        // List preference
        ListPreference listPref = new ListPreference(this);
        listPref.setEntries(R.array.entries_list_preference);
        listPref.setEntryValues(R.array.entryvalues_list_preference);
        listPref.setDialogTitle(R.string.dialog_title_list_preference);
        listPref.setKey("list_preference");
        listPref.setTitle(R.string.title_list_preference);
        listPref.setSummary(R.string.summary_list_preference);
        dialogBasedPrefCat.addPreference(listPref);
        
        // Launch preferences
        PreferenceCategory launchPrefCat = new PreferenceCategory(this);
        launchPrefCat.setTitle(R.string.launch_preferences);
        root.addPreference(launchPrefCat);

        /*
         * The Preferences screenPref serves as a screen break (similar to page
         * break in word processing). Like for other preference types, we assign
         * a key here so that it is able to save and restore its instance state.
         */
        // Screen preference
        PreferenceScreen screenPref = getPreferenceManager().createPreferenceScreen(this);
        screenPref.setKey("screen_preference");
        screenPref.setTitle(R.string.title_screen_preference);
        screenPref.setSummary(R.string.summary_screen_preference);
        launchPrefCat.addPreference(screenPref);
        
        /*
         * You can add more preferences to screenPref that will be shown on the
         * next screen.
         */
        
        // Example of next screen toggle preference
        CheckBoxPreference nextScreenCheckBoxPref = new CheckBoxPreference(this);
        nextScreenCheckBoxPref.setKey("next_screen_toggle_preference");
        nextScreenCheckBoxPref.setTitle(R.string.title_next_screen_toggle_preference);
        nextScreenCheckBoxPref.setSummary(R.string.summary_next_screen_toggle_preference);
        screenPref.addPreference(nextScreenCheckBoxPref);
        
        // Intent preference
        PreferenceScreen intentPref = getPreferenceManager().createPreferenceScreen(this);
        intentPref.setIntent(new Intent().setAction(Intent.ACTION_VIEW)
                .setData(Uri.parse("http://www.android.com")));
        intentPref.setTitle(R.string.title_intent_preference);
        intentPref.setSummary(R.string.summary_intent_preference);
        launchPrefCat.addPreference(intentPref);
        
        // Preference attributes
        PreferenceCategory prefAttrsCat = new PreferenceCategory(this);
        prefAttrsCat.setTitle(R.string.preference_attributes);
        root.addPreference(prefAttrsCat);
        
        // Visual parent toggle preference
        CheckBoxPreference parentCheckBoxPref = new CheckBoxPreference(this);
        parentCheckBoxPref.setTitle(R.string.title_parent_preference);
        parentCheckBoxPref.setSummary(R.string.summary_parent_preference);
        prefAttrsCat.addPreference(parentCheckBoxPref);
        
        // Visual child toggle preference
        TypedArray a = obtainStyledAttributes(android.R.styleable.Theme);
        CheckBoxPreference childCheckBoxPref = new CheckBoxPreference(this);
        childCheckBoxPref.setTitle(R.string.title_child_preference);
        childCheckBoxPref.setSummary(R.string.summary_child_preference);
        childCheckBoxPref.setLayoutResource(
                a.getResourceId(android.R.styleable.Theme_preferenceLayoutChild,
                        0));
        prefAttrsCat.addPreference(childCheckBoxPref);
        a.recycle();
        
        return root;
    }
}
