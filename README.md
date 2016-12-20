# Susy a Drupal Theme
A Drupal theme using the Susy grid system (http://susy.oddbird.net/).

- Enable this theme. 
- Create a sub-theme use: 
<pre>
drush nst "sub_theme_name"
</pre>
- Enable and use the sub-theme. 


###Sass Layout:
- sass/
  - partials/
    - base/   <-- [NO NEED TO EDIT FOLDER BUT CAN]
      - _layout.scss      # Layout
      - _reset.scss       # Reset/normalize
      - _defaults.scss    # Reset/normalize
      - _mixins.scss      # Sass Mixins
    - components/
      - _global.scss      # Gobal
      - _typography.scss  # Typography rules
      - _forms.scss       # Forms
      - _tables.scss      # Tables
    - layout/
      - _grid.scss        # Grid system
      - _header.scss      # Header
      - _footer.scss      # Footer
    - pages/
      - _front.scss       # Home specific styles
      - _basic.scss       # Basic page specific styles
    - utils/
      - _vars.scss        # Sass Variables    <-- [START HERE]
      - _mixins.scss      # Sass Mixins
      - _helpers.scss     # Class & placeholders helpers
  - main.scss                 # Main Sass file
