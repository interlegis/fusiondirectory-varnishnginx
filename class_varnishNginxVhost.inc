<?php
/*
 * This code is part of FusionDirectory (http://www.fusiondirectory.org)
 * Copyright (C) 2012-2013 FusionDirectory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

class varnishNginxVhost extends simplePlugin
{
  var $mainTab = TRUE;

  var $objectclasses = array('top', 'varnishNginxVhost');

  /* Return plugin informations for acl handling  */
  public static function plInfo()
  {
    return array(
      'plShortName'   => _('Varnish/Nginx Vhost'),
      'plDescription' => _('Varnish/Nginx Vhost'),
      'plObjectType'  => array('varnishNginxVhost' => array(
        'name'        => _('Varnish/Ngninx Virtual Host'),
        'filter'      => 'objectClass=varnishNginxVhost',
        'aclCategory' => 'varnishNginx',
        'ou'          => 'ou=varnishNginx,',
        'icon'        => 'plugins/varnishnginx/images/iconMini.png'
      )),

      'plProvidedAcls' => parent::generatePlProvidedAcls(self::getAttributesInfo())
    );
  }

  // The main function : information about attributes
  static function getAttributesInfo ()
  {
    return array(
      // Attributes are grouped by section
      'main' => array(
        'name'  => _('Varnish/Nginx Vhost'),
        'attrs' => array(
          new BaseSelectorAttribute ('ou=varnishNginx,'),
          new HostNameAttribute (_('Vhost'), _('Virtual Host Name'),'cn', TRUE),
          new StringAttribute (_('Backend Path'),_('Path within the specified Backend'), 'backendPath', TRUE, '/' ),
          new BooleanAttribute(_('vHost Monster'), 
                               _('Use Zope Virtual Host Monster URL'),
                               'vHostMonster',
                               TRUE,
                               FALSE),
          new BooleanAttribute(_('HTTPS (SSL)'), 
                               _('Virtual Host use SSL'),
                               'ssl',
                               TRUE,
                               FALSE),
          new SetAttribute (
            new CharSeparatedCompositeAttribute (_('Backends'), 'backends',   
              array(
                new IPv4Attribute (_('IP'),_('IP Address'),'ip',TRUE),
                new IntAttribute (_('Port'),_('Port Number'),'port',TRUE,0,65535,80),
              ),
              ':','','Backends'
            )
          ),
          new TextAreaAttribute (
            _('Remarks'), _('Remarks for the Virtual Host.'),
            'description', FALSE
          )
        )
      )
    );
  }

  function __construct (&$config, $dn = NULL, $object = NULL)
  {
    parent::__construct($config, $dn, $object);

    $this->attributesAccess['cn']->setUnique(TRUE);
  }

  /* Returns the dn of the entry */
  function compute_dn()
  {
    return 'cn='.$this->cn.',ou=varnishNginx,'.$this->base;
  }
}

?>
